#pragma once

#include <memory>

namespace FastMath {
    int round_up(int value, int interval);
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
