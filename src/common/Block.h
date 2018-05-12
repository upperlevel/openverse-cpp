#ifndef OPENVERSE_BLOCK_H
#define OPENVERSE_BLOCK_H

class BlockType {
private:
    const char *id;
    int raw_id;

public:
    explicit BlockType(const char *id);
    ~BlockType();
};

class BlockState; // Todo

#endif
