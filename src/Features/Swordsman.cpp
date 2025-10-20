#include <Swordsman.hpp>

#include <string>
#include <iostream>

namespace sw::features 
{
    Swordsman::Swordsman(int id, const core::Position& position, int health, int strength)
        : Unit(id, position) {
        AddStat(core::UnitStatType::Health, health);
        AddStat(core::UnitStatType::Strength, strength);
	}

	void Swordsman::PerformAction(core::Map& map) {
        if (!IsAlive()) {
            return;
        }

        _current_action.clear();

        const auto adjacent_enemies = FindEnemiesInRange(map, 1, 1);

        // attack
        if (!adjacent_enemies.empty()) {
            PerformCrushingBlow(adjacent_enemies);
            return;
        }

        // move
        if (CanPerformMove()) {
            MoveTo(GetTargetPosition(), map);
            return;
        }

        // idle
        _current_action = "idle. No adjacent_enemies and can't make a move";
    }

	void Swordsman::PerformCrushingBlow(const std::vector<std::shared_ptr<Unit>>& enemies) {
        auto target = SelectRandomUnit(enemies);
        
        if (target == nullptr) {
            std::cout << "Target for not found for CrushingBlow, unit: " << _id;
            return;
        }

        const int damage = GetStatOrDefault(core::UnitStatType::Strength);
        target->TakeDamage(damage);

        _current_action = "Crushing Blow from unit: " + std::to_string(_id) + " to unit " + std::to_string(target->GetId()) + 
            " for " + std::to_string(damage) + " damage";
    }
}
