#pragma once

#include <Core/Unit.hpp>

namespace sw::features 
{
    class Hunter : public core::Unit {
    public:
        Hunter(int id, const core::Position& position, const core::Stats& stats);
        
        void PerformAction(const std::vector<std::shared_ptr<Unit>>& nearby_units) override;
        
    private:
        void PerformSwiftShot(const std::vector<std::shared_ptr<Unit>>& enemies);
        void PerformShadowShot(const std::vector<std::shared_ptr<Unit>>& enemies);
    };
}