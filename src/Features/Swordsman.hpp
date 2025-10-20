#pragma once

#include <Core/Unit.hpp>

namespace sw::features 
{
    class Swordsman : public core::Unit {
    public:
        static constexpr const char* TypeName = "SWORDSMAN";

        Swordsman(int id, const core::Position& pos, int health, int strength);

        void PerformAction(core::Map& map) override;
        std::string GetTypeName() const override { return TypeName; }

    private:
        void PerformCrushingBlow(const std::vector<std::shared_ptr<Unit>>& enemies);
    };
}