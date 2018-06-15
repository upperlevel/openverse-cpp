#pragma once

#include "network.h"

class Player {
private:
    const std::unique_ptr<Connection> connection;
    const std::string name;

public:
    Player(Connection& connection, std::string name);

    /** Gets the connection used by this player. */
    const Connection& get_connection();

    /** Gets the player name. */
    const std::string& get_name();

    /** Sends the packet_data with the given packet_type. */
    void send_packet(PacketType& packet_type, shared_any_ptr packet_data);
};
