#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Core/Position.hpp>

namespace sw::core
{
    class Map;

    enum class UnitStatType {
        Health,
        Strength,
        Agility,
        Range,
    };

    struct Unit {
    protected:
        int _id;
        Position _pos;
        Position _target_pos;
        bool _is_alive;
        bool _can_move;
        bool _can_attack;

        std::unordered_map<UnitStatType, int> _stats; // TODO: вынести всю логику в отдельный класс
    public:
        // TODO: Action
        Unit(int id, const Position& pos, bool can_move = true, bool can_attack = true);
        virtual ~Unit() = default;

        int GetId() const { return _id; };

        void SetPosition(const Position& pos) { _pos = pos; };
        Position GetPosition() const { return _pos; };

        void SetTargetPosition(const Position& target) { _target_pos = target; };
        Position GetTargetPosition() const { return _target_pos; };

        void SetAlive(bool alive) { _is_alive = alive; };
        bool IsAlive() const { return _is_alive; };

        void SetCanMove(bool can_move) { _can_move = can_move; };
        bool IsCanMove() const { return _can_move; };

        void SetCanAttack(bool can_attack) { _can_attack = can_attack; }; 
        bool IsCanAttack() const { return _can_attack; };
        
        void AddStat(UnitStatType type, int value) { _stats.emplace(type, value); }
        bool HasStat(UnitStatType type) const { return _stats.contains(type); }
        int GetStatOrDefault(UnitStatType type, int default_value = 0) const;
        void SetStat(UnitStatType type, int new_value) { _stats[type] = new_value; }

        virtual void TakeDamage(int damage);

        virtual bool CanPerformMove() const;
        virtual Position CalculateNextMove(const Map& map) const;
        virtual bool MoveTo(const Position& target, Map& map);

        virtual void PerformAction(Map& map) = 0;
        virtual std::string GetTypeName() const = 0;

        bool IsAtPosition(const Position& pos) const { return _pos == pos; }
        bool HasTargetPosition() const { return _pos != _target_pos; }

    protected:
        // helpers
        std::vector<std::shared_ptr<Unit>> FindEnemiesInRange(const Map& map, int min_range, int max_range) const;
        std::shared_ptr<Unit> SelectRandomUnit(const std::vector<std::shared_ptr<Unit>>& units) const;
    };
}