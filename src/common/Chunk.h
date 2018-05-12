#ifndef OPENVERSE_CHUNK_H
#define OPENVERSE_CHUNK_H

#include <memory>
#include <unordered_map>

class Block;
class Chunk;
class ChunkPillar;

class Block {
private:
    Chunk *chunk;
    unsigned int rel_x, rel_y, rel_z;

public:
    explicit Block(Chunk *chunk, unsigned int relative_x, unsigned int relative_y, unsigned int relative_z);
    ~Block();

    Chunk *get_chunk();
};


class Chunk {
private:
    int y;

public:
    explicit Chunk(int y);
    ~Chunk();

    int get_y();
    Block *get_block(unsigned int relative_x, unsigned int relative_y, unsigned int relative_z);
};


class ChunkPillar {
private:
    int x, z;
    std::unordered_map<int, Chunk *> chunks;

public:
    explicit ChunkPillar(int x, int z);
    ~ChunkPillar();

    Chunk *get_chunk(int y);
    void set_chunk(Chunk *chunk);
};

#endif
