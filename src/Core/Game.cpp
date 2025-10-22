#include <Core/Game.h>

#include <ranges>
#include <iostream>

#include <Features/Swordsman.hpp>
#include <Features/Hunter.hpp>
#include <Features/UnitFactory.hpp>

namespace sw::core
{
    Game::Game() : _simulation_finished(false), _simulation_round(1) {}

    void Game::CreateMap(int width, int height) {
        _map = std::make_unique<Map>(width, height);
        std::cout << "MAP_CREATED, width=" << width << " height=" << height << std::endl;
    }

    void Game::SpawnUnit(UnitType type, int id, const Position& position, const Stats& stats) {
        if (_units_by_id.contains(id)) {
            std::cout << "ERROR: Unit with id " << id << " already exists" << std::endl;
            return;
        }

        if (_unit_factory == nullptr) {
            std::cout << "ERROR: unit_factory == nullptr" << std::endl;
            return;
        }
        
        const auto unit = _unit_factory->CreateUnit(type, id, position, stats);

        if (_map == nullptr) {
            std::cout << "ERROR: map == nullptr" << std::endl;
            return;
        }

        if (!_map->PlaceUnit(position, unit)) {
            std::cout << "ERROR: Cannot spawn unit at " << position.x << " " << position.y << std::endl;
            return;
        }
        
        _units.push_back(unit);
        _units_by_id.emplace(id, unit);
        
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
            for (const auto& unit : _units) {
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

	void Game::CleanupDeadUnits()
	{
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
        return _units_by_id.contains(id) ? _units_by_id.at(id) : nullptr;
    }

    void Game::ProcessUnitTurn(const std::shared_ptr<Unit>& unit) {
        if (!unit->IsAlive()) {
            return;
        }
        
        const auto center = unit->GetPosition();
        const int min_range = 1;
        const int max_range = std::max(_map->GetWidth(), _map->GetHeight());

        const auto nearby_units = _map->GetUnitsInRange(center, min_range, max_range);

        unit->PerformAction(nearby_units);

        ProcessUnitMovement(unit);
	}

	void Game::ProcessUnitMovement(const std::shared_ptr<Unit>& unit) {
        if (!unit->HasMoveTarget() || !unit->CanPerformMove()) {
            return;
        }
        
        const auto free_cells = GetFreeAdjacentCells(unit);
        Position next_pos = unit->CalculateNextMove(unit->GetTargetPosition(), free_cells);
        
        TryMoveUnitToPosition(unit, next_pos);
	}

	std::vector<Position> Game::GetFreeAdjacentCells(const std::shared_ptr<Unit>& unit) const {
		const auto all_cells = _map->GetValidAdjacentCells(unit->GetPosition());
    
        return all_cells 
            | std::views::filter([this](const Position& pos) { return _map->IsCellFree(pos); })
            | std::ranges::to<std::vector>();
	}

	bool Game::TryMoveUnitToPosition(const std::shared_ptr<Unit>& unit, const Position& target_pos) {
		if (target_pos == unit->GetPosition()) {
            return false; // Already at target position
        }
        
        Position current_pos = unit->GetPosition();

        if (!_map->MoveUnit(current_pos, target_pos)) {
            std::cout << "Error: Failed to move unit to new position";
            return false;
        }
        
        unit->SetPosition(target_pos);

        std::cout << "UNIT_MOVED, unitId=" << unit->GetId() 
                    << " from=(" << current_pos.x << "," << current_pos.y << ")"
                    << " to=(" << target_pos.x << "," << target_pos.y << ")" << std::endl;

        return true;
	}
}
