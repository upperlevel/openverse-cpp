#pragma once

#include <set>
#include <unordered_map>
#include <any>
#include <memory>

#include "util.h"

// -------------------------------- BlockType

class BlockType;

template<class C>
class BlockProperty {
private:
    std::string name;

public:
    explicit BlockProperty(std::string name) {
        this->name = name;
    }

    std::string get_name() const {
        return name;
    }

    virtual std::set<C> get_possible_values() = 0;

    /**
     * Parses the string to its original type.
     * Might throw a std::invalid_argument if parse cannot be done.
     */
    virtual C parse(std::string value) = 0;

    virtual std::string get_value_name(C value) = 0;
};

namespace std {
    template<typename C>
    struct hash<BlockProperty<C>> {
        std::size_t operator()(const BlockProperty<C>& key) const {
            return std::hash<std::string>{}(key.get_name());
        }
    };
}


class IntBlockProperty : public BlockProperty<int> {
private:
    int min, max;
    std::set<int> values;

public:
    explicit IntBlockProperty(std::string name, int min, int max);

    std::set<int> get_possible_values();

    int parse(std::string value);

    std::string get_value_name(int value);
};

class BoolBlockProperty : public BlockProperty<bool> {
private:
    std::set<bool> values = {true, false};

public:
    explicit BoolBlockProperty(std::string name);

    std::set<bool> get_possible_values();

    bool parse(std::string name);

    std::string get_value_name(bool value);
};

/* Todo: fix

class BlockState {
private:
    using Properties = std::unordered_map<std::weak_ptr<BlockProperty<std::any>>, std::any>;
    Properties properties;

    int const index;
    std::weak_ptr<BlockType> const block_type;
    Table<std::weak_ptr<BlockProperty<std::any>>, std::any, BlockState> change_table;

public:
    explicit BlockState(int index, std::shared_ptr<BlockType> block_type, Properties properties) :
            index(index),
            block_type(block_type),
            properties(properties) {}

    template<class T>
    T get_property(BlockProperty<T> &property) {
        auto got = properties.find(&property);
        if (got == properties.end()) {
            throw std::invalid_argument("Property not found");
        }
        return got->second;
    }

    template<class T>
    BlockState &with_property(BlockProperty<T> property, T value) {
        std::optional<BlockState> block_state = change_table.find(property, value);
        if (block_state) {
            // return;
        } else {
            auto curr_val = properties[property];
            if (curr_val == value) {
                return *this;
            }
            // TODO go on bitch
        }
    }
};
*/
