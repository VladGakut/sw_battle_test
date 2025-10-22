#include <Core/Unit.hpp>

#include <iostream>
#include <ranges>
#include <random>

namespace sw::core
{
	Unit::Unit(int id, const Position& pos,  const Stats& stats, bool can_move, bool can_attack) 
        : _id(id), _pos(pos), _stats(stats), _is_alive(true), _can_move(can_move), _can_attack(can_attack) {}

	void Unit::TakeDamage(int damage) {
		if (_stats.health <= 0) {
			std::cout << "Error: Stats::Health not found for unit with id: " << _id;
			return;
		}

		const auto current_hp = _stats.health;
		const auto new_hp = std::max(current_hp - damage, 0);

		_stats.health = new_hp;
			
		if (new_hp <= 0) {
			_is_alive = false;
		}
	}

	bool Unit::CanPerformMove() const {
		return _can_move && _is_alive && HasMoveTarget();
	}

	Position Unit::CalculateNextMove(const Position& target, const std::vector<Position>& available_cells) const {
    	if (!CanPerformMove()) {
			return _pos;
		}

		Position best_pos = _pos;
		int best_distance = _pos.DistanceTo(target);
		
		for (const auto& cell : available_cells) {
			int distance = cell.DistanceTo(target);
			if (distance < best_distance) {
				best_distance = distance;
				best_pos = cell;
			}
		}
		
		return best_pos;
	}

	std::vector<std::shared_ptr<core::Unit>> Unit::GetValidEnemiesInRange(
		const std::vector<std::shared_ptr<core::Unit>>& nearby_units, int min_range, int max_range) const 
	{
		const auto is_in_range = [&id = this->_id, &pos = this->_pos, min_range, max_range](const auto& unit) {
			if (id == unit->GetId() || !unit->IsAlive()) {
				return false;
			}
			
			const int distance = pos.DistanceTo(unit->GetPosition());
			return distance >= min_range && distance <= max_range;
		};

		return nearby_units 
			| std::views::filter(is_in_range) 
			| std::ranges::to<std::vector>();
	}

	std::vector<std::shared_ptr<Unit>> Unit::GetAdjacentValidEnemies(const std::vector<std::shared_ptr<Unit>>& nearby_units) const {
		return GetValidEnemiesInRange(nearby_units, 1, 1);
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
