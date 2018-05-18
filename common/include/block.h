#pragma once

class BlockType {
private:
    const char *id;
    int raw_id;

public:
    explicit BlockType(const char *id);
    ~BlockType();
};

class BlockState; // Todo
