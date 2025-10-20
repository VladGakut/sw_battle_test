#pragma once

#include <Core/Unit.hpp>

namespace sw::features 
{
    class Hunter : public core::Unit {
    public:
        static constexpr const char* TypeName = "Hunter";
        
        Hunter(int id, const core::Position& position, int health, int agility, int strength, int range);
        
        void PerformAction(core::Map& map) override;
        std::string GetTypeName() const override { return TypeName; }
        
    private:
        bool CanSwiftShoot(const core::Map& map) const;
        void PerformSwiftShot(const std::vector<std::shared_ptr<Unit>>& enemies);
        void PerformShadowShot(const std::vector<std::shared_ptr<Unit>>& enemies);
    };
}