#pragma once

#include <network.h>

// ---------------------------------------------------------------- EchoPacketType

class EchoPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

// ---------------------------------------------------------------- LoginPacketType

class LoginPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

// ------------------------------------------------------------------------------------------------ PacketChangeWorldPacketType

class PlayerChangeWorldPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

// ------------------------------------------------------------------------------------------------ ChunkCreatePacketType

/**
 * Contains information that needs a CHUNK_CREATE packet.
 * Chunk position and an array containing blocks full id (type id and meta on a single int).
 */
struct ChunkCreatePacket {
    uint32_t chunk_x, chunk_y, chunk_z;
    uint32_t block_states[16 * 16 * 16];
};

class ChunkCreatePacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

// ------------------------------------------------------------------------------------------------ ChunkDestroyPacketType

/**
 * Contains information that needs a CHUNK_DESTROY packet.
 * Just the position of the chunk to destroy.
 */
struct ChunkDestroyPacket {
    uint32_t chunk_x, chunk_y, chunk_z;
};

class ChunkDestroyPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

// ------------------------------------------------------------------------------------------------ PacketTypes

namespace PacketTypes {
    /**
     * ECHO packet:
     *
     * This is a test packet that holds a 8 bit integer value.
     */
    extern std::shared_ptr<EchoPacketType> ECHO;

    /**
     * LOGIN packet:
     *
     * This packet is sent when the player connects for the first time to the server.
     * So, it's sent from the client to the server and holds the name of the player.
     */
    extern std::shared_ptr<LoginPacketType> LOGIN;

    /**
     * PLAYER_CHANGE_WORLD packet:
     *
     * This packet tells the receiver to change the world. The recipient client
     * will clear its chunks and setup the renderer for a new world.
     * Only holds the world name.
     */
    extern std::shared_ptr<PlayerChangeWorldPacketType> PLAYER_CHANGE_WORLD;

    /**
     * CHUNK_CREATE packet:
     *
     * Creates a chunk at the given position and init it with the given data.
     */
    extern std::shared_ptr<ChunkCreatePacketType> CHUNK_CREATE;

    /**
     * CHUNK_DESTROY packet:
     *
     * Destroys the chunk at the given position.
     */
    extern std::shared_ptr<ChunkDestroyPacketType> CHUNK_DESTROY;
};

/**
 * OPENVERSE_PROTOCOL:
 *
 * A list of packet types associated to the host that sends them.
 */
extern Protocol OPENVERSE_PROTOCOL;
