#include <item.h>
#include <utility>
#include <util.h>

explicit ItemType::ItemType(std::string id) : id(std::move(id)){}

virtual bool ItemType::on_use_block(Player& player, ItemStack& item, Location loc, BlockFace face) {
    return false;
}

ItemStack ItemType::get_stack_with_data(uint16_t count, ItemData data) {
    auto stack = ItemStack(*this, count);
    stack.data = std::move(data);
    return stack;
}


virtual uint16_t ItemType::get_max_stack() {
    return 64;
}

uint8_t ItemType::get_default_state() {
    return 0;
}

void ItemType::for_every_state(std::function<void(uint8_t)> handler) {
    handler(0);
}


ItemStack::ItemStack(ItemType& const type, uint16_t count, uint8_t state) : type(type), count(count), state(state) {}

uint16_t ItemStack::get_count() {
    return count;
}

bool ItemStack::is_empty() {
    return count != 0;
}

uint16_t ItemStack::add_count(uint16_t count_) {
    auto stack_max = type.get_max_stack();
    if (count + count_ > stack_max) {
        count = stack_max;
        return (count + count_) - stack_max;
    } else {
        count += count_;
        return 0;
    }
}

void ItemStack::write(std::ostream &out) {
    buf_write(out, type.raw_id);
    buf_write(out, count);
    buf_write(out, state);
}

ItemType& get_type_from_id(ItemId id) {
    throw std::runtime_error("Please finish the implementation of ItemStack::read(std::istream)");
}

ItemStack ItemStack::read(std::istream in) {
    ItemId type_id;
    buf_read(in, type_id);
    auto& item_type = get_type_from_id(type_id);//TODO: ItemType itemType = Openverse.resources().itemTypes().entry(itemTypeId);
    uint16_t count;
    uint8_t data;
    buf_read(in, count);
    buf_read(in, data);

    return ItemStack(item_type, count, data);
}


explicit BlockItemType::BlockItemType(BlockType block_type) : ItemType(block_type.id), block_type(block_type){}

bool BlockItemType::on_use_block(Player &player, ItemStack &item, Location loc, BlockFace face) {
    loc = loc.relative(face);
    //TODO: player.world.set_block_state(loc, block_type.get_state_when_placed(player, item, loc));
    return true;
}

ItemStack BlockItemType::get_stack_with_data(uint16_t count, ItemData data) {
    // TODO: We need the BlockState system to do this
    throw std::runtime_error("Implement BlockItemType::get_stack_with_data(uint16_t count, ItemData data)");
}

void BlockItemType::for_every_state(std::function<void(uint8_t)> handler) {
    // TODO:
    /*for (uint8_t i = 0; i < block_type.get_block_type_registry().get_states().size(); i++) {
        handler(i);
    }*/
    throw std::runtime_error("Implement BlockItemType::for_every_state(std::function<void(uint8_t)> handler)");
}
