#include <Core/Game.h>

#include <ranges>
#include <iostream>

#include <Features/Swordsman.hpp>
#include <Features/Hunter.hpp>

namespace sw::core
{
    Game::Game() : _simulation_finished(false), _simulation_round(1) {}

    void Game::CreateMap(int width, int height) {
        _map = std::make_unique<Map>(width, height);
        std::cout << "MAP_CREATED, width=" << width << " height=" << height << std::endl;
    }

    void Game::SpawnSwordsman(int id, const Position& position, int health, int strength) {
        if (_units_by_id.contains(id)) {
            std::cout << "ERROR: Unit with id " << id << " already exists" << std::endl;
            return;
        }
        
        const auto swordsman = std::make_shared<features::Swordsman>(id, position, health, strength);
        if (!_map->PlaceUnit(position, swordsman)) {
            std::cout << "ERROR: Cannot spawn unit at " << position.x << " " << position.y << std::endl;
            return;
        }
        
        _units.push_back(swordsman);
        _units_by_id.emplace(id, swordsman);
        
        std::cout << "UNIT_SPAWNED, unitId=" << id << " x=" << position.x << " y=" << position.y << std::endl;
    }

    void Game::SpawnHunter(int id, const Position& position, int health, int agility, int strength, int range) {
        if (_units_by_id.find(id) != _units_by_id.end()) {
            std::cout << "ERROR: Unit with id " << id << " already exists" << std::endl;
            return;
        }
        
        const auto hunter = std::make_shared<features::Hunter>(id, position, health, agility, strength, range);
        if (!_map->PlaceUnit(position, hunter)) {
            std::cout << "ERROR: Cannot spawn unit at " << position.x << " " << position.y << std::endl;
            return;
        }

        _units.push_back(hunter);
        _units_by_id.emplace(id, hunter);
        
        std::cout << "UNIT_SPAWNED, unitId=" << id << " x=" << position.x << " y=" << position.y << std::endl;
    }

    void Game::MarchUnit(int id, const Position& target) {
        const auto unit = GetUnitById(id);
        if (!unit) {
            std::cout << "ERROR: Unit " << id << " not found" << std::endl;
            return;
        }
        
        if (!unit->IsAlive()) {
            std::cout << "ERROR: Unit " << id << " is dead" << std::endl;
            return;
        }
        
        if (!_map->IsValidPosition(target)) {
            std::cout << "ERROR: Invalid target position " << target.x << " " << target.y << std::endl;
            return;
        }
        
        unit->SetTargetPosition(target);
        std::cout << "MARCH_UNIT, id=" << id << " target.x=" << target.x << " target.y=" << target.y << std::endl;
    }

    void Game::RunSimulation() {
        if (!_map) {
            std::cout << "ERROR: Map not created" << std::endl;
            return;
        }
        
        std::cout << "\n---SIMULATION_STARTED---";
        _simulation_round = 1;
        
        while (!IsSimulationFinished()) {
            std::cout << "\nSIMULATION_ROUND_STARTED, round: " << _simulation_round << std::endl;
            
            // Юниты в порядке создания
            for (auto& unit : _units) {
                if (unit->IsAlive()) {
                    ProcessUnitTurn(unit);
                }
            }
            
            CleanupDeadUnits();
            CheckSimulationEndCondition();
            
            std::cout << "SIMULATION_ROUND_ENDED" << std::endl;
            _simulation_round++;
        }
        
        std::cout << "---SIMULATION_ENDED--\n\n";
    }

    void Game::ProcessUnitTurn(const std::shared_ptr<Unit>& unit) {
        unit->PerformAction(*_map);
    }

    void Game::CleanupDeadUnits() {
        std::vector<std::shared_ptr<Unit>> alive_units;
        alive_units.reserve(_units.size());

        for (auto& unit : _units) {
            if (unit->IsAlive()) {
                alive_units.push_back(std::move(unit));
                continue;
            }
            
            int id = unit->GetId();
            Position pos = unit->GetPosition();
                
            _map->RemoveUnit(pos);
            _units_by_id.erase(id);
        }

        alive_units.shrink_to_fit();

        _units = std::move(alive_units);
    }

    void Game::CheckSimulationEndCondition() {
       const auto alive_ids = _units 
            | std::views::filter([](const auto& unit) { return unit->IsAlive(); })
            | std::views::transform([](const auto& unit) { return unit->GetId(); })
            | std::ranges::to<std::vector>();
        
        if (alive_ids.size() > 1) {
            return; // продолжаем дальше играть
        }

        _simulation_finished = true;

        if (alive_ids.empty()) {
            std::cout << "DRAW" << std::endl;
        } else {
            std::cout << "VICTORY! unitId=" << alive_ids[0] << std::endl;
        }
    }

    std::shared_ptr<Unit> Game::GetUnitById(int id) {
        return _units_by_id.contains(id) ? _units_by_id[id] : nullptr;
    }
}