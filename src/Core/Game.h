#pragma once

#include <vector>
#include <unordered_map>

#include <Core/Map.hpp>
#include <Core/Unit.hpp>
#include <IO/System/EventLog.hpp>

namespace sw::core
{
    class Game {
    private:
        std::unique_ptr<Map> _map;
        std::vector<std::shared_ptr<Unit>> _units; // для порядка ходов
        std::unordered_map<int, std::shared_ptr<Unit>> _units_by_id; // для оптимизации
        bool _simulation_finished;
        int _simulation_round;
        std::shared_ptr<EventLog> _event_log;

    public:
        Game();
        
        void SetEventLog(std::shared_ptr<EventLog> event_log) { _event_log = event_log; } 

        void CreateMap(int width, int height);
        void SpawnSwordsman(int id, const Position& position, int health, int strength); // TODO: написать общий интерфейс, чтоб под каждого юнита не плодить новый метод
        void SpawnHunter(int id, const Position& position, int health, int agility, int strength, int range);
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