#pragma once

#include <Core/Unit.hpp>

namespace sw::features 
{
    class Swordsman : public core::Unit {
    public:
        Swordsman(int id, const core::Position& pos, const core::Stats& stats);

        void PerformAction(const std::vector<std::shared_ptr<Unit>>& nearby_units) override;

    private:
        void PerformCrushingBlow(const std::vector<std::shared_ptr<Unit>>& enemies);
    };
}