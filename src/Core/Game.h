#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <Map.hpp>
#include <Unit.hpp>

namespace sw::core
{
    class Game {
    private:
        std::unique_ptr<Map> _map;
        std::vector<std::shared_ptr<Unit>> _units; // для порядка ходов
        std::unordered_map<int, std::shared_ptr<Unit>> _units_by_id; // для оптимизации
        bool _simulation_finished;
        int _current_turn;

    public:
        Game();
        
        void CreateMap(int width, int height);
        void SpawnSwordsman(int id, const Position& position, int health, int strength); // TODO: написать общий интерфейс, чтоб под каждого юнита не плодить новый метод
        // void SpawnHunter(int id, const Position& position, int health, int agility, int strength, int range); // TODO: add hunter
        void MarchUnit(int id, const Position& target);
        
        void RunSimulation();
        bool IsSimulationFinished() const { return _simulation_finished; }
        
        std::shared_ptr<Unit> GetUnitById(int id);
        void CleanupDeadUnits();
        void CheckSimulationEndCondition();
        
    private:
        void ProcessUnitTurn(const std::shared_ptr<Unit>& unit);
    };
}