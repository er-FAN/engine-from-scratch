#pragma once
#include "System.hpp"

namespace step_0001{
    class CollisionSystem : public System
    {
    public:
        CollisionSystem();
        ~CollisionSystem() override;
        
        void Update(EntityStorage &entityStorage, ComponentStorage &componentStorage, float deltaTime) override;
    };

}