#include "protocol.h"

Protocol OPENVERSE_PROTOCOL = {
        { ProtocolSide::EITHER, PacketTypes::ECHO },
        { ProtocolSide::CLIENT, PacketTypes::LOGIN }
};

// --------------------------------------------------------------- EchoPacketType
std::shared_ptr<EchoPacketType> PacketTypes::ECHO = std::make_shared<EchoPacketType>();

shared_any_ptr EchoPacketType::deserialize(std::istream &data) {
    uint32_t val;
    buf_read(data, val);
    return std::make_shared<uint32_t>(val);
}

void EchoPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    buf_write(out, *std::static_pointer_cast<uint32_t>(packet));
}

// --------------------------------------------------------------- LoginPacketType
std::shared_ptr<LoginPacketType> PacketTypes::LOGIN = std::make_shared<LoginPacketType>();

shared_any_ptr LoginPacketType::deserialize(std::istream &in) {
    // Todo: read login player_name
    return 0;
}

void LoginPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    // Todo: write login player_name
}


