#pragma once

#include <Core/IUnitFactory.hpp>

namespace sw::features 
{
    class UnitFactory : public core::IUnitFactory {
        std::shared_ptr<core::Unit> CreateUnit(
            core::UnitType type, int id, 
            const core::Position& pos, const core::Stats& stats
        ) override;
    };
}