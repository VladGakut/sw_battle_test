#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Core/Position.hpp>

namespace sw::core
{
    class Map;

    enum class UnitType {
        Swordsman,
        Hunter
    };

    struct Stats {
        int health{0};
        int strength{0};
        int agility{0};
        int range{0};
    };

    struct Unit {
    protected:
        int _id;
        Position _pos;
        Position _target_pos;
        bool _is_alive;
        bool _can_move;
        bool _can_attack;

        Stats _stats;
    public:
        // TODO: Action
        Unit(int id, const Position& pos, const Stats& stats, bool can_move = true, bool can_attack = true);
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
        
        const Stats& GetStats() const { return _stats; }
        Stats& GetStats() { return _stats; }

        virtual void TakeDamage(int damage);

        virtual bool CanPerformMove() const;
        virtual Position CalculateNextMove(const Position& target, const Map& map) const;
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