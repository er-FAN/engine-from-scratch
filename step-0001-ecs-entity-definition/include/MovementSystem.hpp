#pragma once
#include <iostream>
#include "System.hpp"

namespace step_0001
{
    class MovementSystem : public System
    {
    public:
        MovementSystem();
        ~MovementSystem() override;

        void Update(EntityStorage &entityStorage, ComponentStorage &componentStorage) override;
    };
}