#pragma once
#include <unordered_map>
#include "ComponentArray.hpp"

class Entity;

namespace step_0002
{
    class ComponentManager
    {
    private:
        std::unordered_map<uint64_t, ComponentArray> componentArrays;

    public:
        ComponentManager();
        ~ComponentManager();

        ComponentArray GetComponentArray(uint64_t componentArraySignature);

        uint64_t RegisterComponentAray(uint64_t componentArraySignature);

        std::unordered_map<uint64_t, ComponentArray> GetEntityComponent(Entity entity);
    };
}