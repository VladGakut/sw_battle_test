#pragma once

#include <Core/Unit.hpp>

namespace sw::core
{
    class IUnitFactory {
    public:
        virtual ~IUnitFactory() = default;
        virtual std::shared_ptr<Unit> CreateUnit(UnitType type, int id, const Position& pos, const Stats& stats) = 0;
    };
}