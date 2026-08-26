#pragma once

namespace step_0001
{
    struct EntityStorage;
    struct ComponentStorage;

    class System
    {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void Update(
            EntityStorage& entityStorage,
            ComponentStorage& componentStorage,
            float deltaTime
        ) = 0;
    };
}