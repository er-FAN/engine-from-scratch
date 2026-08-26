#include "Entity.hpp"
#include "Storage.hpp"
#include "MovementSystem.hpp"
#include <chrono>


using namespace step_0001;

int main()
{
    ComponentStorage componentStorage;
    EntityStorage entityStorage;
    SystemStorage systemStorage;

    systemStorage.systems.emplace_back(std::make_unique<MovementSystem>());

    Entity car = {100};
    entityStorage.entities.push_back(car);

    componentStorage.positions[car.Id] = Position{10.0f, 5.0f};
    componentStorage.hasPosition[car.Id] = true;
    componentStorage.sizes[car.Id] = Size{2.0f, 3.0f};
    componentStorage.hasSize[car.Id] = true;
    componentStorage.masses[car.Id] = Mass{200.0f};
    componentStorage.hasMass[car.Id] = true;
    componentStorage.velocities[car.Id] = Velocity{50.0f, 0.0f};
    componentStorage.hasVelocity[car.Id] = true;
    componentStorage.colors[car.Id] = Color{1.0f, 0.0f, 0.0f};
    componentStorage.hasColor[car.Id] = true;
    componentStorage.owners[car.Id] = Owner{"Erfan", 24};
    componentStorage.hasOwner[car.Id] = true;
    componentStorage.capacities[car.Id] = Capacity{5, 120.0f};
    componentStorage.hasCapacity[car.Id] = true;
    componentStorage.specifications[car.Id] = Specification{2025, "Peykan"};
    componentStorage.hasSpecification[car.Id] = true;

    Entity ball = {101};
    entityStorage.entities.push_back(ball);

    componentStorage.balls[ball.Id] = Ball{"football", 0.1f};
    componentStorage.hasBall[ball.Id] = true;
    componentStorage.positions[ball.Id] = Position{30.0f, 0.0f};
    componentStorage.hasPosition[ball.Id] = true;
    componentStorage.sizes[ball.Id] = Size{0.5f, 0.5f};
    componentStorage.hasSize[ball.Id] = true;
    componentStorage.masses[ball.Id] = Mass{2.0f};
    componentStorage.hasMass[ball.Id] = true;
    componentStorage.velocities[ball.Id] = Velocity{5.0f, 7.0f};
    componentStorage.hasVelocity[ball.Id] = true;
    componentStorage.colors[ball.Id] = Color{1.0f, 6.0f, 3.0f};
    componentStorage.hasColor[ball.Id] = true;
    componentStorage.owners[ball.Id] = Owner{"Amirhossein", 25};
    componentStorage.hasOwner[ball.Id] = true;

    auto lastTime = std::chrono::steady_clock::now();
    // Run an infinite loop
    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();

        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        float deltaTime = elapsedTime.count();

        lastTime = currentTime;

        for (auto &system : systemStorage.systems)
        {
            system->Update(entityStorage, componentStorage, deltaTime);
        }
    }
}