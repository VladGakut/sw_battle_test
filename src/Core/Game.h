#pragma once

#include <vector>
#include <unordered_map>

#include <Core/Map.hpp>
#include <Core/Unit.hpp>
#include <Core/IUnitFactory.hpp>

namespace sw::core
{
    class Game {
    private:
        std::unique_ptr<Map> _map;
        std::vector<std::shared_ptr<Unit>> _units; // для порядка ходов
        std::unordered_map<int, std::shared_ptr<Unit>> _units_by_id; // для оптимизации
        std::unique_ptr<IUnitFactory> _unit_factory;
        bool _simulation_finished;
        int _simulation_round;

    public:
        Game();
        
        void CreateMap(int width, int height);
        void SpawnUnit(UnitType type, int id, const Position& position, const Stats& stats);
        void MarchUnit(int id, const Position& target);

        void SetUnitFactory(std::unique_ptr<IUnitFactory> factory) { _unit_factory = std::move(factory); }
        
        void RunSimulation();
        bool IsSimulationFinished() const { return _simulation_finished; }
        
        std::shared_ptr<Unit> GetUnitById(int id);
        void CleanupDeadUnits();
        void CheckSimulationEndCondition();
        
    private:
        void ProcessUnitTurn(const std::shared_ptr<Unit>& unit);
    };
}