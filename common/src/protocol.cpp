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

// -------------------------------------------------------------------------------------------------------------------------------- ChunkCreatePacketType

std::shared_ptr<ChunkCreatePacketType> PacketTypes::CHUNK_CREATE = std::make_shared<ChunkCreatePacketType>();

shared_any_ptr ChunkCreatePacketType::deserialize(std::istream &in) {
    ChunkCreatePacket packet;
    buf_read(in, packet.chunk_x);
    buf_read(in, packet.chunk_y);
    buf_read(in, packet.chunk_z);
    for (int i = 0; i < sizeof(packet.block_states); i++) {
        buf_read(in, packet.block_states[i]);
    }
    return std::make_shared(packet); // Copy packet instance?
}

void ChunkCreatePacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    ChunkCreatePacket raw_packet = *std::static_pointer_cast<ChunkCreatePacket>(packet);
    buf_write(out, raw_packet.chunk_x);
    buf_write(out, raw_packet.chunk_y);
    buf_write(out, raw_packet.chunk_z);
    for (int i = 0; i < sizeof(raw_packet.block_states); i++) {
        buf_write(out, raw_packet.block_states[i]);
    }
}

// -------------------------------------------------------------------------------------------------------------------------------- ChunkDestroyPacketType

std::shared_ptr<ChunkDestroyPacketType> PacketTypes::CHUNK_DESTROY = std::make_shared<ChunkDestroyPacketType>();

shared_any_ptr ChunkDestroyPacketType::deserialize(std::istream &in) {
    ChunkDestroyPacket packet;
    buf_read(in, packet.chunk_x);
    buf_read(in, packet.chunk_y);
    buf_read(in, packet.chunk_z);
    return std::make_shared(packet);
}

void ChunkDestroyPacketType::serialize(const shared_any_ptr packet, std::ostream &out) {
    ChunkDestroyPacket raw_packet = *std::static_pointer_cast<ChunkDestroyPacket>(packet);
    buf_write(out, raw_packet.chunk_x);
    buf_write(out, raw_packet.chunk_y);
    buf_write(out, raw_packet.chunk_z);
}

// -------------------------------------------------------------------------------------------------------------------------------- OpenverseProtocol

Protocol OPENVERSE_PROTOCOL = {
        { ProtocolSide::EITHER, PacketTypes::ECHO },
        { ProtocolSide::CLIENT, PacketTypes::LOGIN },
        { ProtocolSide::SERVER, PacketTypes::PLAYER_CHANGE_WORLD }
};
