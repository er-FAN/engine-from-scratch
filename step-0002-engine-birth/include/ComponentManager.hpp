#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "ComponentArray.hpp"

class Entity;

namespace step_0002
{
    struct ComponentArrayDebugEntry
    {
        uint32_t id;
        std::string name;
        ComponentArray *array;
    };

    class ComponentManager
    {
    private:
        std::unordered_map<uint32_t, ComponentArray> componentArrays;
        std::unordered_map<uint32_t, std::string> componentNames;

    public:
        ComponentManager();
        ~ComponentManager();

        bool EntityCreated();
        bool EntityDestroyed(std::size_t index);

        bool RegisterComponent(
            uint32_t id,
            std::size_t itemSize,
            std::string debugName = {});
        bool AddComponent(std::size_t index, uint32_t componentArrayId, const void *value);
        bool RemoveComponent(uint32_t componentArrayId, std::size_t index);
        bool UpdateComponent(std::size_t index, uint32_t componentArrayId, const void *value);

        // Debug-only inspection API. Entries are snapshots; fetch them again after registration.
        std::vector<ComponentArrayDebugEntry> GetComponentArrays();
    };
}
