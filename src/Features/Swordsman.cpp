#include <Features/Swordsman.hpp>

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
        std::cout << "UNIT_IDLE, id=" << _id << std::endl;
    }

	void Swordsman::PerformCrushingBlow(const std::vector<std::shared_ptr<Unit>>& enemies) {
        auto target = SelectRandomUnit(enemies);
        
        if (target == nullptr) {
            std::cout << "Error: Target for not found for CrushingBlow, unit: " << _id;
            return;
        }

        const int damage = GetStatOrDefault(core::UnitStatType::Strength);
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (CRUSHING_BLOW), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStatOrDefault(core::UnitStatType::Health) << std::endl;

        if (!target->IsAlive()) {
            std::cout << "UNIT_DIED, unitId=" << target->GetId() << std::endl;
        } 
    }
}
