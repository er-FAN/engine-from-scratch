#include "MovementSystem.hpp"
#include "Components.hpp"
#include "ComponentStorage.hpp"

step_0001::MovementSystem::MovementSystem() = default;

step_0001::MovementSystem::~MovementSystem() = default;

void step_0001::MovementSystem::Update(std::uint32_t entityId, ComponentStorage &compStorage)
{
    // Apply movement vector to physical position
    compStorage.positions[entityId].x += compStorage.velocities[entityId].x;
    compStorage.positions[entityId].y += compStorage.velocities[entityId].y;

    // Log output for verification
    std::cout << "[MovementSystem] Entity ID: " << entityId
              << " Updated Position -> ("
              << compStorage.positions[entityId].x << ", " << compStorage.positions[entityId].y << ")\n";
}
