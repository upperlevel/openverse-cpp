#pragma once

#include "doctest.h"
#include "../include/network.h"


TEST_SUITE ("Protocol") {
    class DummyPacket {
    public:
        uint32_t data1;
        uint32_t data2;

        DummyPacket(uint32_t data1, uint32_t data2) : data1(data1), data2(data2) {}
    };

    class DummyPacketType : public PacketType {
    public:

        shared_any_ptr deserialize(std::istream& data) override {
            uint32_t data1, data2;
            buf_read(data, data1);
            buf_read(data, data2);
            return std::make_shared<DummyPacket>(data1, data2);
        }

        void serialize(const shared_any_ptr packet, std::ostream& out) override {
            const std::shared_ptr<DummyPacket> pkt = std::static_pointer_cast<DummyPacket>(packet);
            buf_write(out, pkt->data1);
            buf_write(out, pkt->data2);
        }
    };

    std::string bytes_to_str(std::initializer_list<byte> in) {
        return std::string((char*)in.begin(), in.size());
    }


    void dump_buffer(std::stringbuf &buf) {
        std::string str = buf.str();
        for (auto c : str) {
            std::cout <<
                      std::hex <<
                      std::setw(2) <<
                      std::setfill('0') <<
                      static_cast<unsigned int>(static_cast<unsigned char>(c)) <<
                      std::dec <<
                      std::setw(-1) <<
                      std::endl;
        }
    }

    TEST_CASE ("Single packet type") {
        auto dummy_packet_type = std::make_shared<DummyPacketType>();
        Protocol protocol {
                { ProtocolSide::EITHER, dummy_packet_type },
        };
        FakeSocket socket {ProtocolSide::CLIENT, protocol};

        auto packet = std::make_shared<DummyPacket>(42, 666);

        std::stringbuf buffer;
        std::istream in(&buffer);
        std::ostream out(&buffer);

        socket.serialize_packet(*dummy_packet_type, packet, out);

        CHECK(buffer.str() == std::string{
            0x00, 0x00,
            0x00, 0x00, 0x00, 0x2a,
            0x00, 0x00, 0x02, (char)0x9a,
        });

        auto [packet_type, deserialized_packet, packet_handler] = socket.deserialize_packet(in);

        CHECK(packet_type == dummy_packet_type);
        auto pkt = std::static_pointer_cast<DummyPacket>(deserialized_packet);
        CHECK(pkt->data1 == 42);
        CHECK(pkt->data2 == 666);
    }

    TEST_CASE ("Multiple packet types") {
        auto packet_type1 = std::make_shared<DummyPacketType>();
        auto packet_type2 = std::make_shared<DummyPacketType>();
        auto packet_type3 = std::make_shared<DummyPacketType>();
        auto packet_type4 = std::make_shared<DummyPacketType>();
        Protocol protocol {                             // Global Client Server
                { ProtocolSide::EITHER, packet_type1 }, // 0      0      0
                { ProtocolSide::CLIENT, packet_type2 }, // 1      1      -
                { ProtocolSide::SERVER, packet_type3 }, // 2      -      1
                { ProtocolSide::CLIENT, packet_type4 }, // 3      2      -
        };
        FakeSocket client{ProtocolSide::CLIENT, protocol};
        FakeSocket server{ProtocolSide::SERVER, protocol};

        {// Test client

            auto packet = std::make_shared<DummyPacket>(33, 155);

            std::stringbuf buffer;
            std::istream in(&buffer);
            std::ostream out(&buffer);

            client.serialize_packet(*packet_type4, packet, out);

            CHECK(buffer.str() == bytes_to_str({
                0x00, 0x02,
                0x00, 0x00, 0x00, 0x21,
                0x00, 0x00, 0x00, 0x9b,
            }));

            auto [packet_type, deserialized_packet, packet_handler] = server.deserialize_packet(in);

            CHECK(packet_type == packet_type4);
            auto pkt = std::static_pointer_cast<DummyPacket>(deserialized_packet);
            CHECK(pkt->data1 == 33);
            CHECK(pkt->data2 == 155);

            buffer.pubsetbuf(const_cast<char *>(""), 0);

            client.serialize_packet(*packet_type2, packet, out);
            CHECK(buffer.str() == bytes_to_str({0x00, 0x01, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x9b}));
        }
        {// Test server

            auto packet = std::make_shared<DummyPacket>(192, 168);

            std::stringbuf buffer;
            std::istream in(&buffer);
            std::ostream out(&buffer);

            server.serialize_packet(*packet_type3, packet, out);

            CHECK(buffer.str() == bytes_to_str({0x00, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xa8,}));

            auto [packet_type, deserialized_packet, packet_handler] = client.deserialize_packet(in);

            CHECK(packet_type == packet_type3);
            auto pkt = std::static_pointer_cast<DummyPacket>(deserialized_packet);
            CHECK(pkt->data1 == 192);
            CHECK(pkt->data2 == 168);
        }
    }

    TEST_CASE ("Simple connection") {
        auto dummy_packet_type = std::make_shared<DummyPacketType>();
        Protocol protocol {
                { ProtocolSide::EITHER, dummy_packet_type },
        };
        auto [server_socket, client_socket] = create_socket_pair(protocol);

        // The method get_connection is defined only for the FakeConnection type
        // (as in the normal ServerSocket we wouldn't have only one connection)
        auto server_conn = server_socket->get_connection();
        auto client_conn = client_socket->get_connection();

        std::shared_ptr<DummyPacket> packet_mailbox;

        server_socket->set_handler(*dummy_packet_type, [&packet_mailbox](auto& connection, auto packet) -> void {
            packet_mailbox = std::static_pointer_cast<DummyPacket>(packet);
        });

        auto packet = std::make_shared<DummyPacket>(1234, 5678);
        client_conn->send(*dummy_packet_type, packet);

        CHECK(packet_mailbox->data1 == 1234);
        CHECK(packet_mailbox->data2 == 5678);
    }

    TEST_CASE ("Quite complex connection") {
        auto packet_type1 = std::make_shared<DummyPacketType>();
        auto packet_type2 = std::make_shared<DummyPacketType>();
        auto packet_type3 = std::make_shared<DummyPacketType>();
        auto packet_type4 = std::make_shared<DummyPacketType>();
        auto packet_type5 = std::make_shared<DummyPacketType>();
        auto packet_type6 = std::make_shared<DummyPacketType>();
        Protocol protocol {                             // Global Client Server (Packet IDs)
                { ProtocolSide::EITHER, packet_type1 }, // 0      0      0
                { ProtocolSide::CLIENT, packet_type2 }, // 1      1      -
                { ProtocolSide::SERVER, packet_type3 }, // 2      -      1
                { ProtocolSide::CLIENT, packet_type4 }, // 3      2      -
                { ProtocolSide::CLIENT, packet_type5 }, // 4      3      -
                { ProtocolSide::EITHER, packet_type6 }, // 5      4      2
        };
        auto [server_socket, client_socket] = create_socket_pair(protocol);

        // The method get_connection is defined only for the FakeConnection type
        // (as in the normal ServerSocket we wouldn't have only one connection)
        auto server_conn = server_socket->get_connection();
        auto client_conn = client_socket->get_connection();

        ProtocolSide received_side = ProtocolSide::EITHER;
        std::shared_ptr<PacketType> received_type;
        std::shared_ptr<DummyPacket> received_packet;

        // Setup server handlers
        for (auto& type : {packet_type1, packet_type2, packet_type4, packet_type5, packet_type6}) {
            server_socket->set_handler(*type, [&received_side, &received_type, &received_packet, &type]
                    (auto& conn, auto packet) -> void {
                received_side = ProtocolSide::SERVER;
                received_type = type;
                received_packet = std::static_pointer_cast<DummyPacket>(packet);
            });
        }

        // Setup client handlers
        for (auto& type : {packet_type1, packet_type3, packet_type6}) {
            client_socket->set_handler(*type, [&received_side, &received_type, &received_packet, &type]
                    (auto& conn, auto packet) -> void {
                received_side = ProtocolSide::CLIENT;
                received_type = type;
                received_packet = std::static_pointer_cast<DummyPacket>(packet);
            });
        }

        // ---------------- SETUP DONE -----------------
        // Send some packets to the server
        // packet_type_1
        auto packet = std::make_shared<DummyPacket>(1234, 5678);
        client_conn->send(*packet_type1, packet);
        CHECK(received_side == ProtocolSide::SERVER);
        CHECK(received_type == packet_type1);
        CHECK(received_packet->data1 == 1234);
        CHECK(received_packet->data2 == 5678);

        // packet_type_2
        packet = std::make_shared<DummyPacket>(101010, 010101);
        client_conn->send(*packet_type2, packet);
        CHECK(received_side == ProtocolSide::SERVER);
        CHECK(received_type == packet_type2);
        CHECK(received_packet->data1 == 101010);
        CHECK(received_packet->data2 == 010101);

        // NOTE: the client can't send to the server packet_type_3, check the protocol

        // packet_type_4
        packet = std::make_shared<DummyPacket>(987654, 456789);
        client_conn->send(*packet_type4, packet);
        CHECK(received_side == ProtocolSide::SERVER);
        CHECK(received_type == packet_type4);
        CHECK(received_packet->data1 == 987654);
        CHECK(received_packet->data2 == 456789);

        // packet_type_5
        packet = std::make_shared<DummyPacket>(42244224, 699669);
        client_conn->send(*packet_type5, packet);
        CHECK(received_side == ProtocolSide::SERVER);
        CHECK(received_type == packet_type5);
        CHECK(received_packet->data1 == 42244224);
        CHECK(received_packet->data2 == 699669);

        // packet_type_6
        packet = std::make_shared<DummyPacket>(3, 3);
        client_conn->send(*packet_type6, packet);
        CHECK(received_side == ProtocolSide::SERVER);
        CHECK(received_type == packet_type6);
        CHECK(received_packet->data1 == 3);
        CHECK(received_packet->data2 == 3);

        // Send some other packets to the client
        // packet_type_1
        packet = std::make_shared<DummyPacket>(91827364, 657483920);
        server_conn->send(*packet_type1, packet);
        CHECK(received_side == ProtocolSide::CLIENT);
        CHECK(received_type == packet_type1);
        CHECK(received_packet->data1 == 91827364);
        CHECK(received_packet->data2 == 657483920);

        // NOTE: the client can't send to the client packet_type_2, check the protocol

        // packet_type_3
        packet = std::make_shared<DummyPacket>(0xDEADBEEF, 0xCAFEBABE);
        server_conn->send(*packet_type3, packet);
        CHECK(received_side == ProtocolSide::CLIENT);
        CHECK(received_type == packet_type3);
        CHECK(received_packet->data1 == 0xDEADBEEF);
        CHECK(received_packet->data2 == 0xCAFEBABE);

        // NOTE: the client can't send to the server packet_type_4 nor packet_type_5 check the protocol

        // packet_type_6
        packet = std::make_shared<DummyPacket>(0xBABEFACE, 0xDA7ABA5E);
        server_conn->send(*packet_type6, packet);
        CHECK(received_side == ProtocolSide::CLIENT);
        CHECK(received_type == packet_type6);
        CHECK(received_packet->data1 == 0xBABEFACE);
        CHECK(received_packet->data2 == 0xDA7ABA5E);

        // Sorry for the hex phrases, my random number generator run out of ideas
    }
}
