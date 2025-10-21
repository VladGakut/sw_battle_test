#include <Features/Hunter.hpp>

#include <iostream>

#include <Core/Unit.hpp>

namespace sw::features 
{
    Hunter::Hunter(int id, const core::Position& position, int health, int agility, int strength, int range)
        : Unit(id, position, true, true) {
        AddStat(core::UnitStatType::Health, health);
        AddStat(core::UnitStatType::Agility, agility);
        AddStat(core::UnitStatType::Strength, strength);
        AddStat(core::UnitStatType::Range, range);
    }

    void Hunter::PerformAction(core::Map& map) {
        if (!IsAlive()) {
            return;
        }
        
        // swift attack
        if (CanSwiftShoot(map)) {
            const int range = GetStatOrDefault(core::UnitStatType::Range);
            const auto enemies_in_range = FindEnemiesInRange(map, 2, range);

            PerformSwiftShot(enemies_in_range);
            return;
        }
        
        // shadow attack
        const auto adjacent_enemies = FindEnemiesInRange(map, 1, 1);
        if (!adjacent_enemies.empty()) {
            PerformShadowShot(adjacent_enemies);
            return;
        }
        
        // move
        if (CanPerformMove()) {
            MoveTo(GetTargetPosition(), map);
            return;
        }
        
        // idle
        std::cout << "UNIT_IDLE, unitId=" << _id << std::endl;
    }

    bool Hunter::CanSwiftShoot(const core::Map& map) const {
        const auto adjacent_enemies = FindEnemiesInRange(map, 1, 1);
        if (!adjacent_enemies.empty()) {
            return false;
        }

        const int range = GetStatOrDefault(core::UnitStatType::Range);
        const auto enemies_in_range = FindEnemiesInRange(map, 2, range);
        return !enemies_in_range.empty();
    }

    void Hunter::PerformSwiftShot(const std::vector<std::shared_ptr<Unit>>& enemies) {
        const auto target = SelectRandomUnit(enemies);

        if (target == nullptr || !target->IsAlive()) {
            std::cout << "Error: Swift Shot failed - no valid target" << std::endl;
            return;
        }
        
        const int damage = GetStatOrDefault(core::UnitStatType::Agility);
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (SWIFT_SHOT), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStatOrDefault(core::UnitStatType::Health) << std::endl;

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
        
        const int damage = GetStatOrDefault(core::UnitStatType::Strength);
        target->TakeDamage(damage);

        std::cout << "UNIT_ATTACKED (SHADOW_SHOT), attackerUnitId=" << _id 
            << " targetUnitId=" << target->GetId() 
            << " damage=" << damage 
            << " targetHp=" << target->GetStatOrDefault(core::UnitStatType::Health) << std::endl;

        if (!target->IsAlive()) {
            std::cout << "UNIT_DIED, unitId=" << target->GetId() << std::endl;
        } 
    }
}