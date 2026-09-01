#include "Entity.hpp"
#include "Storage.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "SimpleSDL.hpp"
#include <chrono>


using namespace step_0001;

int main()
{
    ComponentStorage componentStorage;
    EntityStorage entityStorage;
    SystemStorage systemStorage;

    systemStorage.systems.emplace_back(std::make_unique<MovementSystem>());
    systemStorage.systems.emplace_back(std::make_unique<CollisionSystem>());

    Entity car = {100};
    entityStorage.entities.push_back(car);

    componentStorage.tags[car.Id] = Tag{"car"};
    componentStorage.hasTag[car.Id] = true;
    componentStorage.positions[car.Id] = Position{50.0f, 390.0f};
    componentStorage.hasPosition[car.Id] = true;
    componentStorage.sizes[car.Id] = Size{300.0f, 150.0f};
    componentStorage.hasSize[car.Id] = true;
    componentStorage.masses[car.Id] = Mass{200.0f};
    componentStorage.hasMass[car.Id] = true;
    componentStorage.velocities[car.Id] = Velocity{70.0f, 0.0f};
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

    componentStorage.tags[ball.Id] = Tag{"ball"};
    componentStorage.hasTag[ball.Id] = true;
    componentStorage.positions[ball.Id] = Position{100.0f, 0.0f};
    componentStorage.hasPosition[ball.Id] = true;
    componentStorage.sizes[ball.Id] = Size{40.0f, 40.0f};
    componentStorage.hasSize[ball.Id] = true;
    componentStorage.masses[ball.Id] = Mass{2.0f};
    componentStorage.hasMass[ball.Id] = true;
    componentStorage.velocities[ball.Id] = Velocity{200.0f, 200.0f};
    componentStorage.hasVelocity[ball.Id] = true;
    componentStorage.colors[ball.Id] = Color{1.0f, 6.0f, 3.0f};
    componentStorage.hasColor[ball.Id] = true;
    componentStorage.owners[ball.Id] = Owner{"Amirhossein", 25};
    componentStorage.hasOwner[ball.Id] = true;


    simplesdl::App app;

    // راه‌اندازی پنجره
    if (!app.Init("ECS Entity Definition", 1200, 800)) {
        return -1; // خطا در راه‌اندازی
    }

    simplesdl::TextureId ballTexture = app.LoadTexture("assets/ball.svg");
    simplesdl::TextureId carTexture = app.LoadTexture("assets/car.svg");
    simplesdl::TextureId backgroundTexture = app.LoadTexture("assets/background.svg");

    auto lastTime = std::chrono::steady_clock::now();
    bool running = true;
    // Run an infinite loop
    while (running)
    {
        if (app.PollQuit()) {
            running = false;
        }

        auto currentTime = std::chrono::steady_clock::now();

        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        float deltaTime = elapsedTime.count();

        lastTime = currentTime;

        

        for (auto &system : systemStorage.systems)
        {
            system->Update(entityStorage, componentStorage, deltaTime);
        }

        app.Clear({20, 20, 30, 255});
        app.DrawTexture(backgroundTexture,0,0,1200,800);
        for (auto &entity : entityStorage.entities)
        {
            if(componentStorage.hasPosition[entity.Id] && componentStorage.hasSize[entity.Id]){
                auto &pos = componentStorage.positions[entity.Id];
                auto &size = componentStorage.sizes[entity.Id];
                if(componentStorage.hasTag[entity.Id]){
                    if(componentStorage.tags[entity.Id].value == "ball"){
                        app.DrawTexture(ballTexture,pos.x,pos.y,size.width,size.height);
                    }
                    if(componentStorage.tags[entity.Id].value == "car"){
                        app.DrawTexture(carTexture,pos.x,pos.y,size.width,size.height);
                    }
                }
            }
        }
        app.Present();
    }
    return 0;
}