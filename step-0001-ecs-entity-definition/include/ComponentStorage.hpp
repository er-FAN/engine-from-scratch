#pragma once

#include <unordered_map>
#include <cstdint>
#include "Components.hpp"

namespace step_0001
{

    /**
     * @brief Central data container holding component maps for all entities.
     * Uses struct for direct and zero-overhead data access by Systems.
     */
    struct ComponentStorage
    {
        // --- Physical Components ---
        std::unordered_map<std::uint32_t, Position> positions;
        std::unordered_map<std::uint32_t, Size> sizes;
        std::unordered_map<std::uint32_t, Mass> masses;

        // --- Movement Components ---
        std::unordered_map<std::uint32_t, Velocity> velocities;

        // --- Render & Identity Components ---
        std::unordered_map<std::uint32_t, Color> colors;
        std::unordered_map<std::uint32_t, Owner> owners;

        // --- Vehicle Components ---
        std::unordered_map<std::uint32_t, Capacity> capacities;
        std::unordered_map<std::uint32_t, Specification> specifications;

        // --- Ball Components ---
        std::unordered_map<std::uint32_t, Ball> balls;
    };

} // namespace step_0001