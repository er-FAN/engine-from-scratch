#include "MovementSystem.hpp"
#include "Components.hpp"
#include "Storage.hpp"

namespace step_0001
{
    MovementSystem::MovementSystem() = default;

    MovementSystem::~MovementSystem() = default;

    void MovementSystem::Update(EntityStorage &entityStorage, ComponentStorage &componentStorage)
    {
        for (auto &entity : entityStorage.entities)
        {
            auto posIt = componentStorage.positions.find(entity.Id);
            auto velIt = componentStorage.velocities.find(entity.Id);
            if (posIt != componentStorage.positions.end() && velIt != componentStorage.velocities.end())
            {
                posIt->second.x += velIt->second.x * 0.016f;
                posIt->second.y += velIt->second.y * 0.016f;

                std::cout << entity.Id << " : " << posIt->second.x << ", " << posIt->second.y << "\n";
            }
        }
    }
}
