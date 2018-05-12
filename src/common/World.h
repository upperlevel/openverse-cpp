#ifndef OPENVERSE_BLOCKSTATESTORAGE_H
#define OPENVERSE_BLOCKSTATESTORAGE_H

#include <memory>
#include <functional>

#include "Block.h"

// ------------------------------------------------------------------------ BlockStatePalette

class BlockStatePalette {
public:
    virtual unsigned get_id_length() = 0;

    virtual int to_id(BlockState *block_state) = 0;
    virtual BlockState *to_blockstate(unsigned int id) = 0;
};

// ------------------------------------------------------------------------ ArrayBlockStatePalette

class ArrayBlockStatePalette : public BlockStatePalette {
private:
    unsigned id_length;
    unsigned used;
    std::unique_ptr<BlockState *[]> array_ptr;
    std::function<int(int, BlockState *)> on_overflow;

public:
    explicit ArrayBlockStatePalette(unsigned bits, std::function<int(int, BlockState *)>);
    ~ArrayBlockStatePalette();

    unsigned get_id_length() override;
    int to_id(BlockState *block_state) override;
    BlockState *to_blockstate(unsigned int id) override;
};

// TODO: other implementations

// ------------------------------------------------------------------------ VariableBitArray

class VariableBitArray {
private:
    static const unsigned ENTRY_LENGTH = sizeof(char) * 8;

    unsigned int entry_bits;
    unsigned int capacity;
    unsigned long max_value_mask;
    std::unique_ptr<unsigned long[]> array_ptr;

    void check_index(int index);

public:
    explicit VariableBitArray(unsigned int entry_bits, unsigned int capacity);
    ~VariableBitArray();

    unsigned get_capacity();
    unsigned get_entry_bits();
    unsigned long get_max_value();
    unsigned long *get_array();

    unsigned get(unsigned index);
    void set(unsigned index, unsigned value);
};

// ------------------------------------------------------------------------ BlockStateStorage

class BlockStateStorage {
private:
    std::unique_ptr<VariableBitArray> storage;
    std::unique_ptr<BlockStatePalette> palette;
    unsigned palette_bits;

    static unsigned get_index(unsigned x, unsigned y, unsigned z);
    BlockState *get(unsigned index);
    void set(int index, BlockState *state);

public:
    explicit BlockStateStorage();
    ~BlockStateStorage();

    BlockState *get(unsigned x, unsigned y, unsigned z);
    void set(unsigned x, unsigned y, unsigned z, BlockState *state);

    void set_bits_per_palette(unsigned bits);
};

// ------------------------------------------------------------------------ Chunk

class Chunk {
private:
    int y;
    BlockStateStorage blockstate_storage;
    // Todo: other storage: lights, blockentity

public:
    explicit Chunk();
    ~Chunk();

    int get_y();

    BlockStateStorage *get_blockstate_storage();
    BlockState *get_blockstate(unsigned x, unsigned y, unsigned z);
    void set_blockstate(unsigned x, unsigned y, unsigned z, BlockState *blockstate);
};

// ------------------------------------------------------------------------ ChunkProvider

class ChunkProvider {
private:
    std::unordered_map<int, Chunk *> pillar;

public:
    explicit ChunkProvider();
    ~ChunkProvider();

    Chunk *get(int y);
    void load(Chunk *chunk);
    void unload(Chunk *chunk);
};

// ------------------------------------------------------------------------ ChunkPillar

class ChunkPillar {
private:
    std::unique_ptr<ChunkProvider> chunk_provider;
    int x, z;

public:
    explicit ChunkPillar(int x, int z);
    ~ChunkPillar();

    int get_x();
    int get_z();

    ChunkProvider *get_chunk_provider();
    Chunk *get_chunk(int y);
    void load_chunk(Chunk *chunk);
    void unload_chunk(Chunk *chunk);
};

// ------------------------------------------------------------------------ ChunkPillarProvider

class ChunkPillarProvider {
private:
    std::unordered_map<unsigned long, ChunkPillar *> storage;
    static unsigned long get_index(int x, int z);

public:
    explicit ChunkPillarProvider();
    ~ChunkPillarProvider();

    ChunkPillar *get(int x, int z);
    void load(ChunkPillar *chunkpillar);
    void unload(int x, int z);
};

// ------------------------------------------------------------------------ World

class World {
private:
    std::string name;
    std::unique_ptr<ChunkPillarProvider> chunkpillar_provider;

public:
    explicit World(std::string name);
    ~World();

    ChunkPillarProvider *get_chunkpillar_provider();
    ChunkPillar *get_chunkpillar(int x, int z);
    void load_chunkpillar(ChunkPillar *chunkpillar);
    void unload_chunkpillar(int x, int z);
};

#endif
