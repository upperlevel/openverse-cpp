#pragma once

#include <memory>
#include <unordered_map>

#include "chunk_pillar.h"

// ---------------------------------------------------------------- World

/**
 * A world is basically a grid of chunk pillars with a name.
 * Works like a ChunkPillarProvider.
 */
class World {
private:
    const std::string name;
    std::unordered_map<unsigned long, std::shared_ptr<ChunkPillar>> storage;

public:
    explicit World(std::string name);

    /**
     * Returns the name of this world.
     */
    const std::string& get_name();

    /**
     * Returns a ChunkPillar at the given position from the internal storage.
     * If any ChunkPillar is found creates a new one (currently).
     */
    std::weak_ptr<ChunkPillar> get_chunk_pillar(int x, int z);
};
