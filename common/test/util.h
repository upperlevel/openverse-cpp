#pragma once

#include <limits>

#include "doctest.h"
#include "../include/util.h"

TEST_SUITE("VariableBitArray") {
    TEST_CASE("Simple") {
        VariableBitArray array(2, 4);
        array.set(0, 3);
        array.set(1, 2);
        array.set(2, 1);
        array.set(3, 0);

        CHECK(array.get(0) == 3);
        CHECK(array.get(1) == 2);
        CHECK(array.get(2) == 1);
        CHECK(array.get(3) == 0);
        CHECK(0b00011011 == (array.get_array()[0] & 0xff));
    }

    TEST_CASE("Out of bounds") {
        VariableBitArray array(2, 4);
        CHECK_THROWS(array.set(5, 4));
    }

    TEST_CASE("Other") {
        VariableBitArray array(8 * sizeof(int), 8);
        long c = (std::numeric_limits<int>::max() - std::numeric_limits<int>::min()) / 8;
        for (int i = 0; i < 8; i++) {
            array.set(i, (int) c * i);
        }
        for (int i = 0; i < 8; i++) {
            CHECK(c * i == array.get(i));
        }
    }

    TEST_CASE("Odd length") {
        VariableBitArray array(25, 5);
        int tests[] = {41847297, 1549070137, 123014888, 235795758, 5972058}; // random typed numbers
        for (int i = 0; i < sizeof(tests) / sizeof(int); i++) {
            tests[i] &= array.get_max_value();
            array.set(i, tests[i]);
        }
        for (int i = 0; i < sizeof(tests) / sizeof(int); i++) {
            CHECK(tests[i] == array.get(i));
        }
    }
}

TEST_SUITE("Table") {
    TEST_CASE("General") {
        // insert
        Table<int, int, long> table;
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                table.insert(x, y, x + y - 100);
            }
        }
        // find
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                if (x < 0 || y < 0) {
                    // out of range keys, should fail
                    CHECK(!table.find(x, y));
                } else {
                    CHECK(table.find(x, y));
                }
            }
        }
        // erase
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                if (x < 0 || y < 0) {
                    // should not find those
                    CHECK(!table.erase(x, y));
                } else {
                    CHECK(table.erase(x, y));
                }

            }
        }
        // insert
        CHECK(!table.find(0, 0)); // searches an erased element
        table.insert(0, 0, 123);
        CHECK(table.find(0, 0)); // now should find it
    }
}
