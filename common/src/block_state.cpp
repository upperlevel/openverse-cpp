#include "block_state.h"

// ---------------------------------------------------------------- IntBlockProperty

IntBlockProperty::IntBlockProperty(std::string name, int min, int max) : BlockProperty(name) {
    if (min < 0 || max <= min) {
        throw std::invalid_argument("Negative min or max lower than min");
    }
    this->min = min;
    this->max = max;
    for (int i = min; i <= max; i++) {
        values.insert(i);
    }
}

std::set<int> IntBlockProperty::get_possible_values() {
    return values;
}

int IntBlockProperty::parse(std::string value) {
    int res = std::stoi(value);
    if (res < min || res >= max) {
        throw std::out_of_range("Value not inbounds");
    }
    return res;
}

std::string IntBlockProperty::get_value_name(int value) {
    return std::to_string(value);
}

// ---------------------------------------------------------------- BoolBlockProperty

BoolBlockProperty::BoolBlockProperty(std::string name) : BlockProperty(name) {
}

std::set<bool> BoolBlockProperty::get_possible_values() {
    return values;
}

bool BoolBlockProperty::parse(std::string value) {
    if (value == "true") {
        return true;
    } else if (value == "false") {
        return false;
    } else {
        throw std::invalid_argument("Value should be true or false");
    }
}

std::string BoolBlockProperty::get_value_name(bool value) {
    return value ? "true" : "false";
}

// Todo: EnumProperty
