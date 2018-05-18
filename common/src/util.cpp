#include "util.h"

// FastMath

int FastMath::round_up(int value, int interval) {
    if (value == 0) {
        return interval;
    }
    int i = value & interval;
    return i == 0 ? value : value + interval - i;
}

// VariableBitArray

void VariableBitArray::check_index(int index) {
    if (index > capacity) {
        throw std::out_of_range("Index out of range");
    }
}

VariableBitArray::VariableBitArray(int entry_bits, int capacity) {
    this->entry_bits = entry_bits;
    this->capacity = capacity;
    this->array = std::make_unique<long *>(new long[FastMath::round_up(entry_bits * capacity, ENTRY_LENGTH) / ENTRY_LENGTH]);
    this->max_value_mask = (1ul << entry_bits) - 1; // ul = unsigned long
}

VariableBitArray::~VariableBitArray() = default;

int VariableBitArray::get_entry_bits() {
    return entry_bits;
}

int VariableBitArray::get_capacity() {
    return capacity;
}

long *VariableBitArray::get_array() {
    return *array;
}

long VariableBitArray::get_max_value() {
    return max_value_mask;
}

int VariableBitArray::get(int index) {
    check_index(index);
    int bit_index = index * entry_bits;
    int real_index = bit_index / ENTRY_LENGTH;
    int bit_offset = bit_index % ENTRY_LENGTH;

    int written = ENTRY_LENGTH - bit_offset;
    if (written >= entry_bits) {
        return int(((*array)[real_index] >> bit_offset) & max_value_mask);
    } else {
        return int((((*array)[real_index] >> bit_offset) | ((*array)[real_index + 1] << written)) & max_value_mask);
    }
}

void VariableBitArray::set(int index, int value) {
    check_index(index);
    int bit_index = index * entry_bits;
    int real_index = bit_index / ENTRY_LENGTH;
    int bit_offset = bit_index % ENTRY_LENGTH;

    (*array)[real_index] = (*array)[real_index] & ~(max_value_mask << bit_offset) | (value & max_value_mask) << bit_offset;

    int written = ENTRY_LENGTH - bit_offset;
    if (written < entry_bits) {
        int i = real_index + 1;
        (*array)[i] = (*array)[i] & ~(max_value_mask >> written) | (value & max_value_mask) >> written;
    }
}
