#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include "location.h"
#include "block.h"
#include "network.h"

using ItemId = uint32_t;
using ItemData = std::unordered_map<std::string, std::any;


class Player;
class ItemStack;
class ItemStack;

class ItemType {
public:
    static ItemType AIR {"air"};
private:
    std::string const id;

public:
    ItemId raw_id;

    ItemType(const ItemType&) = delete;
    ItemType& operator=(const ItemType&) = delete;

    explicit ItemType(std::string id);


    virtual bool on_use_block(Player& player, ItemStack& item, Location loc, BlockFace face);

    virtual ItemStack get_stack_with_data(uint16_t count, ItemData data);

    virtual uint16_t get_max_stack();

    virtual uint8_t get_default_state();

    virtual void for_every_state(std::function<void(uint8_t)> handler);
};

class ItemStack {
public:
    static ItemStack const EMPTY {ItemType::AIR, 0, 0};

private:
    ItemType& type;
    uint16_t count;
    uint8_t state;

public:
    ItemData data;

    explicit ItemStack(ItemType& type, uint16_t count=1, uint8_t state=0);

    bool is_empty();

    uint16_t get_count();

    uint16_t add_count(uint16_t count);

    void write(std::ostream& out);

    static ItemStack read(std::istream in);
};


class BlockItemType : public ItemType {
public:
    BlockType& block_type;

    explicit BlockItemType(BlockType block_type);

    bool on_use_block(Player& player, ItemStack& item, Location loc, BlockFace face) override;

    ItemStack get_stack_with_data(uint16_t count, ItemData data) override;

    void for_every_state(std::function<void(uint8_t)> handler) override;
};
