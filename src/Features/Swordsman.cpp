#include <Features/Swordsman.hpp>

#include <string>
#include <iostream>

namespace sw::features 
{
    Swordsman::Swordsman(int id, const core::Position& position, const core::Stats& stats)
        : Unit(id, position, stats) {}

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
            MoveTo(_target_pos, map);
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

        const int damage = _stats.strength;
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (CRUSHING_BLOW), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStats().health << std::endl;

        if (!target->IsAlive()) {
            std::cout << "UNIT_DIED, unitId=" << target->GetId() << std::endl;
        } 
    }
}
