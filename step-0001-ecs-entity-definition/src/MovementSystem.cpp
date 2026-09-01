#include "MovementSystem.hpp"
#include "Components.hpp"
#include "Storage.hpp"

namespace step_0001
{
    MovementSystem::MovementSystem() = default;

    MovementSystem::~MovementSystem() = default;

    void MovementSystem::Update(EntityStorage &entityStorage, ComponentStorage &componentStorage, float deltaTime)
    {
        for (auto &entity : entityStorage.entities)
        {
            if (componentStorage.hasPosition[entity.Id] && componentStorage.hasVelocity[entity.Id])
            {
                componentStorage.positions[entity.Id].x += componentStorage.velocities[entity.Id].x * deltaTime;
                componentStorage.positions[entity.Id].y += componentStorage.velocities[entity.Id].y * deltaTime;
            }
        }
    }
}
