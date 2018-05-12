#include "Chunk.h"

// Block

Block::Block(Chunk *chunk, unsigned int rel_x, unsigned int rel_y, unsigned int rel_z) {
    this->chunk = chunk;
    this->rel_x = rel_x;
    this->rel_y = rel_y;
    this->rel_z = rel_z;
}

Block::~Block() = default;

Chunk *Block::get_chunk() {
    return chunk;
}

// Chunk

Chunk::Chunk(const int y) {
    this->y = y;
}

Chunk::~Chunk() = default;

int Chunk::get_y() {
    return y;
}

Block *Chunk::get_block(unsigned int relative_x, unsigned int relative_y, unsigned int relative_z) {
    return new Block(this, relative_x, relative_y, relative_z);
}

// ChunkPillar

ChunkPillar::ChunkPillar(int x, int z) : x(x), z(z) {
}

ChunkPillar::~ChunkPillar() = default;

Chunk *ChunkPillar::get_chunk(int y) {
    auto found = chunks.find(y);
    if (found == chunks.end()) {
        return nullptr;
    }
    return found->second;
}

void ChunkPillar::set_chunk(Chunk *chunk) {
    chunks.insert(std::make_pair(chunk->get_y(), chunk));
}
