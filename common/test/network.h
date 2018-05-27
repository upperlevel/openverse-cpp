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

        std::shared_ptr<void> deserialize(std::istream& data) override {
            uint32_t data1, data2;
            buf_read(data, data1);
            buf_read(data, data2);
            return std::make_shared<DummyPacket>(data1, data2);
        }

        void serialize(const std::shared_ptr<void> packet, std::ostream& out) override {
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
        Connection connection {ProtocolSide::CLIENT, protocol};

        auto packet = std::make_shared<DummyPacket>(42, 666);

        std::stringbuf buffer;
        std::istream in(&buffer);
        std::ostream out(&buffer);

        connection.serialize_packet(*dummy_packet_type, packet, out);

        CHECK(buffer.str() == std::string{
            0x00, 0x00,
            0x00, 0x00, 0x00, 0x2a,
            0x00, 0x00, 0x02, (char)0x9a,
        });

        auto type_packet_pair = connection.deserialize_packet(in);

        CHECK(type_packet_pair.first == dummy_packet_type);
        auto pkt = std::static_pointer_cast<DummyPacket>(type_packet_pair.second);
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
        Connection client_conn{ProtocolSide::CLIENT, protocol};
        Connection server_conn{ProtocolSide::SERVER, protocol};

        {// Test client

            auto packet = std::make_shared<DummyPacket>(33, 155);

            std::stringbuf buffer;
            std::istream in(&buffer);
            std::ostream out(&buffer);

            client_conn.serialize_packet(*packet_type4, packet, out);

            CHECK(buffer.str() == bytes_to_str({
                0x00, 0x02,
                0x00, 0x00, 0x00, 0x21,
                0x00, 0x00, 0x00, 0x9b,
            }));

            auto type_packet_pair = server_conn.deserialize_packet(in);

            CHECK(type_packet_pair.first == packet_type4);
            auto pkt = std::static_pointer_cast<DummyPacket>(packet);
            CHECK(pkt->data1 == 33);
            CHECK(pkt->data2 == 155);

            buffer.pubsetbuf(const_cast<char *>(""), 0);

            client_conn.serialize_packet(*packet_type2, packet, out);
            CHECK(buffer.str() == bytes_to_str({0x00, 0x01, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x9b}));
        }
        {// Test server

            auto packet = std::make_shared<DummyPacket>(192, 168);

            std::stringbuf buffer;
            std::istream in(&buffer);
            std::ostream out(&buffer);

            server_conn.serialize_packet(*packet_type3, packet, out);

            CHECK(buffer.str() == bytes_to_str({0x00, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xa8,}));

            auto type_packet_pair = client_conn.deserialize_packet(in);

            CHECK(type_packet_pair.first == packet_type3);
            auto pkt = std::static_pointer_cast<DummyPacket>(packet);
            CHECK(pkt->data1 == 192);
            CHECK(pkt->data2 == 168);
        }
    }
}
