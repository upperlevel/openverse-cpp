#pragma once

#include <memory>
#include <unordered_map>

#include "chunk.h"

/**
 * A ChunkPillar is a column (along y-axis) that contains an unlimited number of chunks.
 * Works as a ChunkProvider.
 */
class ChunkPillar {
private:
    const int x, z;
    std::unordered_map<int, std::shared_ptr<Chunk>> storage;

public:
    explicit ChunkPillar(int x, int z);

    const int& get_x();
    const int& get_z();

    /**
     * Gets a chunk along pillar by the given y position.
     * @return the Chunk
     */
    std::weak_ptr<Chunk> get_chunk(int y);

    /**
     * Sets the chunk on this pillar.
     * @param chunk the Chunk
     */
    void set_chunk(Chunk& chunk);

    /**
     * Clears the chunk at the given position.
     * @param y its position
     * @return true if the chunk has been cleared
     */
    bool clear_chunk(int y);
};
