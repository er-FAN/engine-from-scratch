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

    /**
     * @brief Central data container holding component maps for all entities.
     * Uses struct for direct and zero-overhead data access by Systems.
     */
    struct ComponentStorage
    {
        // --- Physical Components ---
        Position positions[MAX_ENTITIES];
        Size sizes[MAX_ENTITIES];
        Mass masses[MAX_ENTITIES];

        // --- Movement Components ---
        Velocity velocities[MAX_ENTITIES];

        // --- Render & Identity Components ---
        Color colors[MAX_ENTITIES];
        Owner owners[MAX_ENTITIES];

        // --- Vehicle Components ---
        Capacity capacities[MAX_ENTITIES];
        Specification specifications[MAX_ENTITIES];

        // --- Ball Components ---
        Ball balls[MAX_ENTITIES];
    };

    struct EntityStorage 
    {
        Entity entities[MAX_ENTITIES];
    };

    struct SystemStorage
    {
        std::vector<std::unique_ptr<System>> systems;
    };
    

} // namespace step_0001