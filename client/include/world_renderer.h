#pragma once

#include <string>

#include "world.h"
#include "chunk.h"
#include "network.h"

// ---------------------------------------------------------------- WorldPacketListener

/**
 * Listens for world packets and updates the world.
 */
class WorldPacketListener {
public:
    WorldPacketListener(World &world, Socket& socket);
};

// ---------------------------------------------------------------- WorldPacketListener


