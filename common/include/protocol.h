#pragma once

#include <network.h>

class EchoPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

class LoginPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

class PlayerChangeWorldPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

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
};

/**
 * OPENVERSE_PROTOCOL:
 *
 * A list of packet types associated to the host that sends them.
 */
extern Protocol OPENVERSE_PROTOCOL;
