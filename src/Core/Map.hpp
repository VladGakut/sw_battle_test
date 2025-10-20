#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <Unit.hpp>

namespace sw::core
{
	class Map
	{
    private:
        int _width;
        int _height;
        std::unordered_map<Position, std::shared_ptr<Unit>, PositionHash> _unit_by_position;

    public:
        Map(int width, int height);

        int GetWidth() const { return _width; }
        int GetHeight() const { return _height; }

        bool IsValidPosition(const Position& pos) const;

        // Unit management
        bool IsCellFree(const Position& pos) const;
        std::shared_ptr<Unit> GetUnitAt(const Position& pos);
        bool PlaceUnit(const Position& pos, const std::shared_ptr<Unit>& unit);
        bool RemoveUnit(const Position& pos) { return _unit_by_position.erase(pos) > 0; }
        bool MoveUnit(const Position& from, const Position& to);

        std::vector<Position> GetValidAdjacentCells(const Position& pos, int radius = 1) const;

        std::vector<std::shared_ptr<Unit>> GetUnitsInRange(const Position& center, int minRange, int maxRange) const;

        size_t GetUnitCount() const { return _unit_by_position.size(); }
        void Clear() { _unit_by_position.clear(); }
	};
}
