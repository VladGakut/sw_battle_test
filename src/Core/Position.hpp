#pragma once

#include <functional> // для std::hash
#include <utility>

namespace sw::core
{
    struct Position {
        int x;
        int y;
    
        Position(int x = 0, int y = 0) : x(x), y(y) {}

        int DistanceTo(const Position& other) const { 
            return std::max(std::abs(x - other.x), std::abs(y - other.y));
        }
    
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
        
        bool operator!=(const Position& other) const {
            return !(*this == other);
        }
    };

    struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
    }
};
}
