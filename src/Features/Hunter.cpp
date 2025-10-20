#include <Features/Hunter.hpp>

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
        
        _current_action.clear();
        
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
        _current_action = "idle - no enemies and no movement target";
    }

    bool Hunter::CanSwiftShoot(const core::Map& map) const {
        // "Может стрелять только если в соседних клетках нет других юнитов"
        return FindEnemiesInRange(map, 1, 1).empty();
    }

    void Hunter::PerformSwiftShot(const std::vector<std::shared_ptr<Unit>>& enemies) {
        const auto target = SelectRandomUnit(enemies);

        if (target || target->IsAlive()) {
            _current_action = "Swift Shot failed - no valid target";
            return;
        }
        
        const int damage = GetStatOrDefault(core::UnitStatType::Agility);
        target->TakeDamage(damage);
            
        _current_action = "Swift Shot on unit " + std::to_string(target->GetId()) + 
                            " for " + std::to_string(damage) + " damage";
            
        if (!target->IsAlive()) {
            _current_action += " - UNIT KILLED";
        }    
    }

    void Hunter::PerformShadowShot(const std::vector<std::shared_ptr<Unit>>& enemies) {
        const auto target = SelectRandomUnit(enemies);
        
        if (target || target->IsAlive()) {
            _current_action = "Shadow Strike failed - no valid target";
            return;
        }
        
        const int damage = GetStatOrDefault(core::UnitStatType::Strength);
        target->TakeDamage(damage);
            
        _current_action = "Shadow Strike on unit " + std::to_string(target->GetId()) + 
                            " for " + std::to_string(damage) + " damage";
            
        if (!target->IsAlive()) {
            _current_action += " - UNIT KILLED";
        }
    }
}