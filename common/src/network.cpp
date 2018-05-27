#include "network.h"


Protocol::Protocol(std::initializer_list<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> types) : packetTypes(types) {
    PacketId nextId = 0;
    for (auto& id_and_type : packetTypes) {
        id_and_type.second->assign_id(nextId++);
    }
}

Connection::Connection(ProtocolSide side, const Protocol &protocol) : side(side) {
    global_id_to_local_id_map.reserve(protocol.packetTypes.size());
    PacketId nextSendId = 0;
    for (auto& sideAndType : protocol.packetTypes) {
        PacketId localId;
        if (sideAndType.first == side || sideAndType.first == ProtocolSide::EITHER) {
            localId = nextSendId++;// Assign local id
        } else {
            localId = MAX_PACKET_ID;// Undefined (sort of)
        }
        global_id_to_local_id_map.emplace_back(localId);

        if (sideAndType.first != side) {// Matches both the inverse and EITHER sides
            receive_id_to_type_map.emplace_back(sideAndType.second);
        }
    }
}

std::pair<std::shared_ptr<PacketType>, std::shared_ptr<void>> Connection::deserialize_packet(std::istream &data) {
    PacketId id;
    buf_read(data, id);
    if (id >= receive_id_to_type_map.size()) {
        throw std::runtime_error("Invalid packet id: " + id);
    }
    std::shared_ptr<PacketType>& type = receive_id_to_type_map[id];
    std::shared_ptr<void> ptr = type->deserialize(data);
    return std::make_pair(type, ptr);
}

void Connection::serialize_packet(PacketType &type, std::shared_ptr<void> packet, std::ostream &out) {
    if (type.get_id() >= global_id_to_local_id_map.size()) {
        throw std::runtime_error("Invalid packet type: " + type.get_id());
    }
    buf_write(out, global_id_to_local_id_map[type.get_id()]);
    type.serialize(std::move(packet), out);
}
