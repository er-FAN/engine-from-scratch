#pragma once
#include <cstdint>

namespace step_0001 {

    /**
     * @brief Represents a unique entity identifier in the ECS architecture.
     * 
     * @details In ECS, an Entity holds no data or behavior; it is strictly an ID.
     * Data is stored in Components, and logic is handled by Systems.
     */
    struct Entity {
        std::uint32_t Id = 0; ///< Unique numerical identifier for the entity
    };

} // namespace step_0001