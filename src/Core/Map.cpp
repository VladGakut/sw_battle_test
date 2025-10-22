#include <Core/Map.hpp>

#include <unordered_map>

namespace sw::core
{
	Map::Map(int width, int height) : _width(width), _height(height) {}

    bool Map::IsValidPosition(const Position& pos) const {
        return pos.x >= 0 && pos.x < _width && pos.y >= 0 && pos.y < _height;
    }

    bool Map::IsCellFree(const Position& pos) const {
        return IsValidPosition(pos) && !_unit_by_position.contains(pos);    
    }

    std::shared_ptr<Unit> Map::GetUnitAt(const Position& pos) const {
        return _unit_by_position.contains(pos)
            ? _unit_by_position.at(pos)
            : nullptr;
    }

    bool Map::PlaceUnit(const Position& pos, const std::shared_ptr<Unit>& unit) {
        if (!IsCellFree(pos)) {
            return false;
        }
        
        _unit_by_position.emplace(pos, unit);

        return true;
    }
    
    bool Map::MoveUnit(const Position& from, const Position& to) {
        const auto unit = GetUnitAt(from);

        if (!unit || !IsCellFree(to)) {
            return false;
        }

        _unit_by_position.erase(from);

        return PlaceUnit(to, unit);;
	}

	std::vector<Position> Map::GetValidAdjacentCells(const Position& pos, int radius) const {
	    std::vector<Position> cells;
    
        for (int x = -radius; x <= radius; ++x) {
            for (int y = -radius; y <= radius; ++y) {
                if (x == 0 && y == 0) continue; // Пропускаем центральную клетку
                
                Position cell(pos.x + x, pos.y + y);
                
                if (IsValidPosition(cell)) {
                    cells.push_back(cell);
                }
            }
        }
        
        return cells;
	}

	std::vector<std::shared_ptr<Unit>> Map::GetUnitsInRange(const Position& center, int minRange, int maxRange) const {
	    std::vector<std::shared_ptr<Unit>> unitsInRange;
        unitsInRange.reserve(_unit_by_position.size());
    
        for (const auto& [pos, unit] : _unit_by_position) {
            if (!unit->IsAlive()) {
                continue;
            }
                
            int distance = center.DistanceTo(pos);
            if (distance >= minRange && distance <= maxRange) {
                unitsInRange.push_back(unit);
            }
        }

        unitsInRange.shrink_to_fit();
            
        return unitsInRange;
	}
}
