#include "protocol.h"

#include <iostream>

// -------------------------------------------------------------------------------------------------------------------------------- EchoPacketType

std::shared_ptr<EchoPacketType> PacketTypes::ECHO = std::make_shared<EchoPacketType>();

shared_any_ptr EchoPacketType::deserialize(std::istream &in) {
    uint32_t val;
    buf_read(in, val);
    return std::make_shared<uint32_t>(val);
}

void EchoPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    buf_write(out, *std::static_pointer_cast<uint32_t>(packet));
}

// -------------------------------------------------------------------------------------------------------------------------------- LoginPacketType

std::shared_ptr<LoginPacketType> PacketTypes::LOGIN = std::make_shared<LoginPacketType>();

shared_any_ptr LoginPacketType::deserialize(std::istream &in) {
    // Todo buf_string_read
    uint8_t c;
    std::string player_name;
    while (true) {
        buf_read(in, c);
        if (c == '\0') {
            break;
        }
        player_name += c;
    }
    return std::make_shared<std::string>(player_name);
}

void LoginPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    // Todo buf_string_write
    for (char& c : *std::static_pointer_cast<std::string>(packet)) {
        buf_write(out, (uint8_t) c);
    }
    buf_write(out, (uint8_t) '\0');
}

// -------------------------------------------------------------------------------------------------------------------------------- PlayerChangeWorldPacket

std::shared_ptr<PlayerChangeWorldPacketType> PacketTypes::PLAYER_CHANGE_WORLD = std::make_shared<PlayerChangeWorldPacketType>();

shared_any_ptr PlayerChangeWorldPacketType::deserialize(std::istream &in) {
    return std::make_shared<std::string>(buf_read_string(in));
}

void PlayerChangeWorldPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    buf_write_string(out, *std::static_pointer_cast<std::string>(packet));
}

// -------------------------------------------------------------------------------------------------------------------------------- OpenverseProtocol
Protocol OPENVERSE_PROTOCOL = {
        { ProtocolSide::EITHER, PacketTypes::ECHO },
        { ProtocolSide::CLIENT, PacketTypes::LOGIN },
        { ProtocolSide::SERVER, PacketTypes::PLAYER_CHANGE_WORLD }
};
