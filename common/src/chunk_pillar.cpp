#include "chunk_pillar.h"

// ------------------------------------------------------------------------------------------------ ChunkPillar

ChunkPillar::ChunkPillar(int x, int z) : x(x), z(z) {}

const int& ChunkPillar::get_x() {
    return x;
}

const int& ChunkPillar::get_z() {
    return z;
}

std::weak_ptr<Chunk> ChunkPillar::get_chunk(int y) {
    auto got = storage.find(y);
    if (got == storage.end()) {
        return nullptr;
    }
    return std::weak_ptr(got->second);
}

void ChunkPillar::set_chunk(Chunk& chunk) {
    storage.insert(std::pair(chunk.get_y(), std::shared_ptr(&chunk)));
}

bool ChunkPillar::clear_chunk(int y) {
    return storage.erase(y) > 0;
}
