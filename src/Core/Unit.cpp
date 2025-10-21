#include <Core/Unit.hpp>

#include <iostream>
#include <random>

#include <Core/Map.hpp>

namespace sw::core
{
	Unit::Unit(int id, const Position& pos, bool can_move, bool can_attack) 
        : _id(id), _pos(pos), _is_alive(true), _can_move(can_move), _can_attack(can_attack) {}

	int Unit::GetStatOrDefault(UnitStatType type, int default_value) const {
		return _stats.contains(type) ? _stats.at(type) : default_value;
	}

	void Unit::TakeDamage(int damage) {
		if (!HasStat(UnitStatType::Health)) {
			std::cout << "Error: UnitStatType::Health not found for unit with id: " << _id;
			return;
		}


		const auto current_hp = GetStatOrDefault(UnitStatType::Health);
		const auto new_hp = std::max(current_hp - damage, 0);

		SetStat(UnitStatType::Health, new_hp);
			
		if (new_hp <= 0) {
			_is_alive = false;
		}
	}

	bool Unit::CanPerformMove() const {
		return _can_move && _is_alive && HasTargetPosition();
	}

	Position Unit::CalculateNextMove(const Map& map) const {
    	if (!CanPerformMove()) {
			return false;
		}

		auto cells = map.GetValidAdjacentCells(_pos);
		Position best_pos = _pos;
		int best_distance = _pos.DistanceTo(_target_pos);
		
		for (const auto& cell : cells) {
			if (map.IsCellFree(cell)) {
				int distance = cell.DistanceTo(_target_pos);
				if (distance < best_distance) {
					best_distance = distance;
					best_pos = cell;
				}
			}
		}
		
		return best_pos;
	}

	bool Unit::MoveTo(const Position& target, Map& map) {

		if (!CanPerformMove()) {
			std::cout << "Error: Can't perform the movement";
			return false;
		}
    
		Position next_pos = CalculateNextMove(map);
		if (next_pos != _pos && map.MoveUnit(_pos, next_pos)) {
			_pos = next_pos;

			std::cout << "UNIT_MOVE_TO, unitId=" << _id << " x=" << _pos.x << " y=" << _pos.y << std::endl;
			return true;
		}
		
		return false;
	}

	std::vector<std::shared_ptr<Unit>> Unit::FindEnemiesInRange(const Map& map, int min_range, int max_range) const {
		const auto units_in_range = map.GetUnitsInRange(_pos, min_range, max_range);
		std::vector<std::shared_ptr<Unit>> enemies;
		
		for (const auto& unit : units_in_range) {
			if (unit.get() != this && unit->IsAlive()) {
				enemies.push_back(unit);
			}
		}
		
		return enemies;
	}

	std::shared_ptr<Unit> Unit::SelectRandomUnit(const std::vector<std::shared_ptr<Unit>>& units) const {
		if (units.empty()) {
			return nullptr;
		}
    
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, units.size() - 1);
		
		return units[dis(gen)];
	}
}
