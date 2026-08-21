#include "Entity.hpp"
#include "Storage.hpp"
#include "MovementSystem.hpp"
#include <thread>

int main()
{
    using namespace step_0001;

    ComponentStorage componentStorage;
    EntityStorage entityStorage;
    SystemStorage systemStorage;

    systemStorage.systems.emplace_back(std::make_unique<MovementSystem>());

    Entity car = {100};
    entityStorage.entities.push_back(car);

    componentStorage.positions[car.Id] = Position{10.0f, 5.0f};
    componentStorage.sizes[car.Id] = Size{2.0f, 3.0f};
    componentStorage.masses[car.Id] = Mass{200.0f};
    componentStorage.velocities[car.Id] = Velocity{50.0f, 0.0f};
    componentStorage.colors[car.Id] = Color{1.0f, 0.0f, 0.0f};
    componentStorage.owners[car.Id] = Owner{"Erfan", 24};
    componentStorage.capacities[car.Id] = Capacity{5, 120.0f};
    componentStorage.specifications[car.Id] = Specification{2025, "Peykan"};

    Entity ball = {101};
    entityStorage.entities.push_back(ball);

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

        // Pause the execution of the current thread for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}