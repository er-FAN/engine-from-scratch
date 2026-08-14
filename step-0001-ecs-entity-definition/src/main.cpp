#include "Entity.hpp"
#include "Components.hpp"
#include "ComponentStorage.hpp"
#include "MovementSystem.hpp"
#include <thread>

int main()
{
    using namespace step_0001;

    ComponentStorage compStorage;
    MovementSystem move;

    Entity car = {100};

    compStorage.positions[car.Id] = Position{10.0f, 5.0f};
    compStorage.sizes[car.Id] = Size{2.0f, 3.0f};
    compStorage.masses[car.Id] = Mass{200.0f};
    compStorage.velocities[car.Id] = Velocity{50.0f, 0.0f};
    compStorage.colors[car.Id] = Color{1.0f, 0.0f, 0.0f};
    compStorage.owners[car.Id] = Owner{"Erfan", 24};
    compStorage.capacities[car.Id] = Capacity{5, 120.0f};
    compStorage.specifications[car.Id] = Specification{2025, "Peykan"};

    Entity ball = {101};
    compStorage.balls[ball.Id] = Ball{"football", 0.1f};
    compStorage.positions[ball.Id] = Position{30.0f, 0.0f};
    compStorage.sizes[ball.Id] = Size{0.5f, 0.5f};
    compStorage.masses[ball.Id] = Mass{2.0f};
    compStorage.velocities[ball.Id] = Velocity{5.0f, 7.0f};
    compStorage.colors[ball.Id] = Color{1.0f, 6.0f, 3.0f};
    compStorage.owners[ball.Id] = Owner{"Amirhossein", 25};

    

    while (true)
    {
        // Run an infinite loop
        while (true)
        {
            move.Update(car.Id, compStorage);
            move.Update(ball.Id, compStorage);

            // Pause the execution of the current thread for 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}