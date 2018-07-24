#pragma once

#include <memory>
#include <functional>

#include "util.h"
#include "block.h"

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
    std::unique_ptr<BlockState **> array;
    std::function<int(unsigned, BlockState *)> on_overflow;

public:
    explicit ArrayBlockStatePalette(unsigned bits, std::function<int(unsigned, BlockState *)>);
    ~ArrayBlockStatePalette();

    unsigned get_id_length() override;
    int to_id(BlockState *block_state) override;
    BlockState *to_blockstate(unsigned int id) override;
};

// TODO: other implementations

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
