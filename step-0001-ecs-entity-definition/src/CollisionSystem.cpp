#include "CollisionSystem.hpp"
#include "Components.hpp"
#include "Storage.hpp"

namespace step_0001
{
    CollisionSystem::CollisionSystem() = default;
    CollisionSystem::~CollisionSystem() = default;

    void CollisionSystem::Update(EntityStorage &entityStorage, ComponentStorage &componentStorage, float deltaTime)
    {
        for (auto &entity : entityStorage.entities)
        {
            if(componentStorage.positions[entity.Id].x + componentStorage.sizes[entity.Id].width >= 1200 || componentStorage.positions[entity.Id].x  < 0){
                componentStorage.velocities[entity.Id].x *= -1;
            }
            if(componentStorage.positions[entity.Id].y  + componentStorage.sizes[entity.Id].height >= 800 || componentStorage.positions[entity.Id].y < 0){
                componentStorage.velocities[entity.Id].y *= -1;
            }
        }
    }
}

