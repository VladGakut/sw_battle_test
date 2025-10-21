#include <Features/UnitFactory.hpp>

#include <iostream>

#include <Features/Swordsman.hpp>
#include <Features/Hunter.hpp>

namespace sw::features 
{
    std::shared_ptr<core::Unit> UnitFactory::CreateUnit(
        core::UnitType type, int id, const core::Position& pos, const core::Stats& stats)
    {
        switch (type) {
            case sw::core::UnitType::Swordsman:
                return std::make_shared<Swordsman>(id, pos, stats);
            case sw::core::UnitType::Hunter:
                return std::make_shared<Hunter>(id, pos, stats);
            default:
                throw std::invalid_argument("Unknown unit type");
        }    
    }
}
