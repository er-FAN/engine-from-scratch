#pragma once
#include <iostream>

namespace step_0001
{
    /**
     * @brief Central storage and management container for all entity components.
     *
     * @details ComponentStorage acts as the core database in the ECS architecture,
     * storing component data mapped to entities and allowing systems to query and
     * iterate over entities with specific component combinations.
     */
    class ComponentStorage;

    /**
     * @brief System responsible for updating entity positions based on their velocity.
     *
     * @details The MovementSystem queries the ComponentStorage for all entities that possess
     * both Position and Velocity components, applying movement logic
     * frame by frame.
     */
    class MovementSystem
    {
    public:
        MovementSystem();
        ~MovementSystem();

        /**
         * @brief Updates the positions of all eligible entities.
         *
         * @param entityId The unique identifier of the target entity to update.
         * @param compStorage Reference to the central storage containing all active components.
         */
        void Update(std::uint32_t entityId, ComponentStorage &compStorage);
    };
}