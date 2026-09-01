#pragma once

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <memory>
#include "Components.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace step_0001
{
    const std::uint32_t MAX_ENTITIES = 1000;
    /**
     * @brief Central data container holding component maps for all entities.
     * Uses struct for direct and zero-overhead data access by Systems.
     */
    struct ComponentStorage
    {
        // --- Physical Components ---
        Position positions[MAX_ENTITIES];
        bool hasPosition[MAX_ENTITIES]{};
        Size sizes[MAX_ENTITIES];
        bool hasSize[MAX_ENTITIES]{};
        Mass masses[MAX_ENTITIES];
        bool hasMass[MAX_ENTITIES]{};

        // --- Movement Components ---
        Velocity velocities[MAX_ENTITIES];
        bool hasVelocity[MAX_ENTITIES]{};

        // --- Render & Identity Components ---
        Color colors[MAX_ENTITIES];
        bool hasColor[MAX_ENTITIES]{};
        Owner owners[MAX_ENTITIES];
        bool hasOwner[MAX_ENTITIES]{};

        // --- Vehicle Components ---
        Capacity capacities[MAX_ENTITIES];
        bool hasCapacity[MAX_ENTITIES]{};
        Specification specifications[MAX_ENTITIES];
        bool hasSpecification[MAX_ENTITIES]{};

        // --- Ball Components ---
        Tag tags[MAX_ENTITIES];
        bool hasTag[MAX_ENTITIES]{};
    };

    struct EntityStorage
    {
        std::vector<Entity> entities;
    };

    struct SystemStorage
    {
        std::vector<std::unique_ptr<System>> systems;
    };

} // namespace step_0001