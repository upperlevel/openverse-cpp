#include "network.h"

void buf_write_string(std::ostream& out, std::string string) {
    for (char c : string) {
        buf_write(out, (uint8_t) c);
    }
    buf_write(out, (uint8_t) '\0');
}

std::string buf_read_string(std::istream& in) {
    std::string res;
    while (true) {
        uint8_t c;
        buf_read(in, c);
        res += (char) c;
        if (c == '\0') {
            break;
        }
    }
    return res;
}

Protocol::Protocol(std::initializer_list<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> types) : packet_types(types) {
    PacketId nextId = 0;
    for (auto& id_and_type : packet_types) {
        id_and_type.second->assign_id(nextId++);
    }
}

Socket::Socket(ProtocolSide side, const Protocol &protocol) : side(side) {
    global_id_to_local_id_map.reserve(protocol.packet_types.size());
    PacketId nextSendId = 0;
    for (auto& sideAndType : protocol.packet_types) {
        PacketId localId;
        if (sideAndType.first == side || sideAndType.first == ProtocolSide::EITHER) {
            localId = nextSendId++;// Assign local id
        } else {
            localId = MAX_PACKET_ID;// Undefined (sort of)
        }
        global_id_to_local_id_map.emplace_back(localId);

        if (sideAndType.first != side) {// Matches both the inverse and EITHER sides
            receive_id_to_type_map.emplace_back(std::make_pair(sideAndType.second, nullptr));
        }
    }
}

std::tuple<std::shared_ptr<PacketType>, shared_any_ptr, PacketHandler> Socket::deserialize_packet(std::istream &data) {
    PacketId id;
    buf_read(data, id);
    if (id >= receive_id_to_type_map.size()) {
        throw std::runtime_error("Invalid packet id: " + id);
    }
    std::pair<std::shared_ptr<PacketType>, PacketHandler>& type_and_handler = receive_id_to_type_map[id];
    shared_any_ptr ptr = type_and_handler.first->deserialize(data);
    return {type_and_handler.first, ptr, type_and_handler.second};
}

void Socket::serialize_packet(PacketType &type, shared_any_ptr packet, std::ostream &out) {
    if (type.get_id() >= global_id_to_local_id_map.size()) {
        throw std::runtime_error("Invalid packet type: " + type.get_id());
    }
    buf_write(out, global_id_to_local_id_map[type.get_id()]);
    type.serialize(std::move(packet), out);
}

std::optional<PacketId> Socket::search_type_local_id(PacketType& type) {
    PacketId target_id = type.get_id();
    // Binary search

    unsigned long left = 0;
    auto right = receive_id_to_type_map.size();
    while (left <= right) {
        auto middle = left + (right - left)/2;
        int id = receive_id_to_type_map[middle].first->get_id();

        if (id == target_id) {
            return static_cast<PacketId>(middle);
        }
        if (id < target_id) {
            left = middle + 1;
        } else { // id < target_id
            right = middle - 1;
        }
    }
    return std::nullopt;
}

void Socket::set_handler(PacketType& type, PacketHandler handler) {
    auto local_id = search_type_local_id(type);
    if (!local_id) {
        throw std::runtime_error("Invalid type");
    }
#ifndef NDEBUG
    if (receive_id_to_type_map[*local_id].second) {
        throw std::runtime_error("Handler already specified");
    }
#endif // NDEBUG
    receive_id_to_type_map[*local_id].second = std::move(handler);
}

void Connection::on_receive_packet(std::istream &data) {
    auto [packet_type, packet_ptr, packet_handler] = socket.deserialize_packet(data);
    packet_handler(*this, packet_ptr);
}

// ---------------------------------------------------------------- FakeNetwork ----------------------------------------------------------------

std::pair<std::unique_ptr<FakeSocket>, std::unique_ptr<FakeSocket>> create_socket_pair(Protocol& protocol) {
    std::unique_ptr<FakeSocket> server = std::make_unique<FakeSocket>(ProtocolSide::SERVER, protocol);
    std::unique_ptr<FakeSocket> client = std::make_unique<FakeSocket>(ProtocolSide::CLIENT, protocol);
    server->get_connection()->receiver = client->get_connection();
    client->get_connection()->receiver = server->get_connection();
    return {std::move(server), std::move(client)};
}

FakeConnection::FakeConnection(FakeSocket& socket) : Connection(socket){};

void FakeConnection::on_receive_packet(PacketId local_id, shared_any_ptr& packet) {
    auto [packet_type, packet_handler] = socket.receive_id_to_type_map[local_id];
    packet_handler(*this, packet);
}

void FakeConnection::send(PacketType& type, shared_any_ptr packet) {
    auto ptr = receiver.lock();
    if (!ptr) {
        throw std::runtime_error("Connection closed");
    }
    auto send_id =  socket.global_id_to_local_id_map[type.get_id()];
#ifndef NDEBUG
    if (send_id == MAX_PACKET_ID) {
        throw std::runtime_error("Cannot send packet: Protocol Error");
    }
#endif // NDEBUG
    ptr->on_receive_packet(send_id, packet);
}

FakeSocket::FakeSocket(ProtocolSide side, Protocol &protocol) : Socket(side, protocol) {
    connection = std::make_shared<FakeConnection>(*this);
    connections = {connection};
};
