#include "Entity.hpp"
#include "Storage.hpp"
#include "MovementSystem.hpp"
#include <thread>

using namespace step_0001;

const std::uint32_t MAX_ENTITIES = 1000;

int main()
{
    ComponentStorage componentStorage;
    EntityStorage entityStorage;
    SystemStorage systemStorage;

    systemStorage.systems.emplace_back(std::make_unique<MovementSystem>());

    Entity car = {1};
    entityStorage.entities[car.Id] = car;

    componentStorage.positions[car.Id] = Position{10.0f, 5.0f};
    componentStorage.sizes[car.Id] = Size{2.0f, 3.0f};
    componentStorage.masses[car.Id] = Mass{200.0f};
    componentStorage.velocities[car.Id] = Velocity{50.0f, 0.0f};
    componentStorage.colors[car.Id] = Color{1.0f, 0.0f, 0.0f};
    componentStorage.owners[car.Id] = Owner{"Erfan", 24};
    componentStorage.capacities[car.Id] = Capacity{5, 120.0f};
    componentStorage.specifications[car.Id] = Specification{2025, "Peykan"};

    Entity ball = {2};
    entityStorage.entities[ball.Id] = ball;

    componentStorage.balls[ball.Id] = Ball{"football", 0.1f};
    componentStorage.positions[ball.Id] = Position{30.0f, 0.0f};
    componentStorage.sizes[ball.Id] = Size{0.5f, 0.5f};
    componentStorage.masses[ball.Id] = Mass{2.0f};
    componentStorage.velocities[ball.Id] = Velocity{5.0f, 7.0f};
    componentStorage.colors[ball.Id] = Color{1.0f, 6.0f, 3.0f};
    componentStorage.owners[ball.Id] = Owner{"Amirhossein", 25};

    // Run an infinite loop
    while (true)
    {
        for (auto &system : systemStorage.systems)
        {
            system->Update(entityStorage, componentStorage);
        }

        // Pause the execution of the current thread for 16 milli second (60 fps)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}