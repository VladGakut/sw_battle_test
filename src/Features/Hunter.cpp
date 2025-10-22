#include <Features/Hunter.hpp>

#include <iostream>

namespace sw::features 
{
    Hunter::Hunter(int id, const core::Position& position, const core::Stats& stats)
        : Unit(id, position, stats, true, true) {
    }

    void Hunter::PerformAction(const std::vector<std::shared_ptr<Unit>>& nearby_units) {
        if (!IsAlive()) {
            return;
        }
        
        const int min_range = 2;
        const int max_range = _stats.range;

        const auto adjacent_enemies = GetAdjacentValidEnemies(nearby_units);
        const auto enemies_in_range = GetValidEnemiesInRange(nearby_units, min_range, max_range);

        // swift attack
        if (adjacent_enemies.empty() && !enemies_in_range.empty()) {
            PerformSwiftShot(enemies_in_range);
            return;
        }
        
        // shadow attack
        if (!adjacent_enemies.empty()) {
            PerformShadowShot(adjacent_enemies);
            return;
        }
        
        // idle
        std::cout << "UNIT_IDLE, unitId=" << _id << std::endl;
    }

    void Hunter::PerformSwiftShot(const std::vector<std::shared_ptr<Unit>>& enemies) {
        const auto target = SelectRandomUnit(enemies);

        if (target == nullptr || !target->IsAlive()) {
            std::cout << "Error: Swift Shot failed - no valid target" << std::endl;
            return;
        }
        
        const int damage = _stats.agility;
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (SWIFT_SHOT), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStats().health << std::endl;

        if (!target->IsAlive()) {
            std::cout << "UNIT_DIED, unitId=" << target->GetId() << std::endl;
        } 
    }

    void Hunter::PerformShadowShot(const std::vector<std::shared_ptr<Unit>>& enemies) {
        const auto target = SelectRandomUnit(enemies);
        
        if (target == nullptr || !target->IsAlive()) {
            std::cout << "Error: Shadow Strike failed - no valid target";
            return;
        }
        
        const int damage = _stats.strength;
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (SHADOW_SHOT), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStats().health << std::endl;

        if (!target->IsAlive()) {
            std::cout << "UNIT_DIED, unitId=" << target->GetId() << std::endl;
        } 
    }
}