#pragma once


#include "block.h"

class Location {
public:
    int x, y, z;

    Location(int x, int y, int z) : x(x), y(y), z(z){};

    Location(BlockFace face) : x(0), y(0), z(0) {
        operator=(relative(face));
    }


    Location operator+(Location o) const noexcept {
        return Location(x + o.x, y + o.y, z + o.z);
    }

    void operator+=(Location o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
    }

    Location operator-(Location o) const noexcept {
        return Location(x - o.x, y - o.y, z - o.z);
    }

    void operator-=(Location o) noexcept {
        x -= o.x;
        y -= o.y;
        z -= o.z;
    }

    Location operator/(int v) const noexcept {
        return Location(x / v, y / v, z / v);
    }

    void operator/=(int v) noexcept {
        x /= v;
        y /= v;
        z /= v;
    }

    Location operator/(Location o) const noexcept {
        return Location(x / o.x, y / o.y, z / o.z);
    }

    void operator/=(Location o) {
        x /= o.x;
        y /= o.y;
        y /= o.z;
    }

    Location operator*(int v) const noexcept {
        return Location(x * v, y * v, z * v);
    }

    void operator*=(int v) noexcept {
        x *= v;
        y *= v;
        z *= v;
    }

    Location operator*(Location o) const noexcept {
        return Location(x * o.x, y * o.y, z * o.z);
    }

    void operator*=(Location o) {
        x *= o.x;
        y *= o.y;
        y *= o.z;
    }

    Location relative(BlockFace face, int r = 1) const noexcept {
        switch (face) {
            case BlockFace::UP:
                return Location(x, y + r, z);
            case BlockFace::DOWN:
                return Location(x, y - r, z);
            case BlockFace::FRONT:
                return Location(x, y, z - r);
            case BlockFace::BACK:
                return Location(x, y, z + r);
            case BlockFace::RIGHT:
                return Location(x + r, y, z);
            case BlockFace::LEFT:
                return Location(x - r, y, z);
        }
    }
};

