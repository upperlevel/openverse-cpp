#pragma once

#include <memory>
#include <unordered_map>
#include <optional>

namespace FastMath {
    int round_up(int value, int interval);

    template <typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }

    template <typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }
}

class VariableBitArray {
private:
    static const int ENTRY_LENGTH = sizeof(char) * 8;

    int entry_bits;
    int capacity;
    long max_value_mask;
    std::unique_ptr<long *> array;

    void check_index(int index);

public:
    explicit VariableBitArray(int entry_bits, int capacity);
    ~VariableBitArray();

    int get_capacity();
    int get_entry_bits();
    long get_max_value();
    long *get_array();

    int get(int index);
    void set(int index, int value);
};

template<typename R, typename C, typename V>
class Table {
private:
    std::unordered_map<R, std::unique_ptr<std::unordered_map<C, V>>> wrapped;

public:
    void insert(R row_key, C column_key, V value) {
        auto row_it = wrapped.find(row_key);
        if (row_it == wrapped.end()) {
            // if the row was not added, inserts a new one
            auto row = std::make_unique<std::unordered_map<C, V>>(); // automatically allocates an unordered_map on the heap
            row->insert(std::make_pair(column_key, value));
            wrapped.insert(std::make_pair(row_key, std::move(row))); // std::move required here because unique_ptr would be copied
        } else {
            // if the row is found, adds the element to this
            row_it->second->insert(std::make_pair(column_key, value));
        }
    }

    bool erase(R row_key, C column_key) {
        auto row_it = wrapped.find(row_key);
        if (row_it == wrapped.end()) {
            // if row is not found, there is nothing to search more
            return false;
        }
        auto &row = row_it->second;
        auto col_it = row->find(column_key);
        if (col_it == row->end()) {
            return false;
        }
        col_it = row->erase(col_it);
        if (col_it == row->end()) {
            // if after removing the element the row is empty, removes it
            wrapped.erase(row_it);
        }
        return true;
    }

    std::optional<V> find(R row_key, C column_key) {
        auto row_it = wrapped.find(row_key);
        if (row_it == wrapped.end()) {
            return std::nullopt;
        }
        auto &row = row_it->second;
        auto col_it = row->find(column_key);
        if (col_it == row->end()) {
            return std::nullopt;
        }
        return std::make_optional(col_it->second);
    }
};
