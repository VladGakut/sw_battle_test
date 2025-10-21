#include <Core/Game.h>

#include <ranges>
#include <iostream>

#include <Features/Swordsman.hpp>
#include <Features/Hunter.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitSpawned.hpp>

namespace sw::core
{
    Game::Game() : _simulation_finished(false), _simulation_round(1) {
        _event_log = std::make_shared<EventLog>();
    }

    void Game::CreateMap(int width, int height) {
        _map = std::make_unique<Map>(width, height);
        _event_log->log(_simulation_round, io::MapCreated{(uint)width, (uint)height});
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
        
       _event_log->log(_simulation_round, io::UnitSpawned{(uint)id, swordsman->GetTypeName(), (uint)position.x, (uint)position.y});
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
        
        _event_log->log(_simulation_round, io::UnitSpawned{(uint)id, hunter->GetTypeName(), (uint)position.x, (uint)position.y});
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
        _event_log->log(_simulation_round, io::MarchStarted{(uint)id, (uint)unit->GetPosition().x, (uint)unit->GetPosition().y, 
            (uint)target.x, (uint)target.y
        });
    }

    void Game::RunSimulation() {
        if (!_map) {
            std::cout << "ERROR: Map not created" << std::endl;
            return;
        }
        
        std::cout << "SIMULATION_STARTED" << std::endl;
        
        while (!IsSimulationFinished()) {
            std::cout << "SIMULATION_ROUND_STARTED " << _simulation_round << std::endl;
            
            // Юниты в порядке создания
            for (auto& unit : _units) {
                if (unit->IsAlive()) {
                    ProcessUnitTurn(unit);
                }
            }
            
            CleanupDeadUnits();
            CheckSimulationEndCondition();
            
            std::cout << "SIMULATION_ROUND_ENDED " << _simulation_round << std::endl;
            _simulation_round++;
        }
        
        std::cout << "SIMULATION_ENDED" << std::endl;
    }

    void Game::ProcessUnitTurn(const std::shared_ptr<Unit>& unit) {
        std::cout << "ProcessUnitTurn Started with unit_id: " << unit->GetId() << std::endl;
        unit->PerformAction(*_map);

        if (!unit->GetCurrentAction().empty()) {
            std::cout << "UNIT_ACTION unit_id: " << unit->GetId() << " " << unit->GetCurrentAction() << std::endl;
        }

        std::cout << "ProcessUnitTurn Ended" << std::endl;
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
                
            _event_log->log(_simulation_round, io::UnitDied{(uint)id});
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
            std::cout << "VICTORY! id:" << alive_ids[0] << std::endl;
        }
    }

    std::shared_ptr<Unit> Game::GetUnitById(int id) {
        return _units_by_id.contains(id) ? _units_by_id[id] : nullptr;
    }
}