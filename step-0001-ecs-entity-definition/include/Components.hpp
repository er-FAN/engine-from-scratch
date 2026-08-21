#pragma once

#include <string>
#include <cstdint>

/**
 * @file Components.hpp
 * @brief Base component definitions.
 */

namespace step_0001
{
    struct Position
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Size
    {
        float width = 0.0f;
        float height = 0.0f;
    };

    struct Mass
    {
        float massKg = 0.0f; /**< Mass in kilograms. */
    };

    /**
     * @brief Stores linear velocity vector in 2D space.
     */
    struct Velocity
    {
        float x = 0.0f; /**< Velocity along the X-axis. */
        float y = 0.0f; /**< Velocity along the Y-axis. */
    };

    /**
     * @brief Normalized RGB color representation (0.0f to 256.0f).
     */
    struct Color
    {
        float r = 0.0f; /**< Red channel intensity. */
        float g = 0.0f; /**< Green channel intensity. */
        float b = 0.0f; /**< Blue channel intensity. */
    };

    /**
     * @brief Holds information of a ball.
     */
    struct Ball
    {
        std::string type = "football"; /**< Type of ball. */
        float radius = 0.0f; /**< Radius of ball. */
    };
    

    /**
     * @brief Represents the owner details of an entity.
     */
    struct Owner
    {
        std::string name;     /**< Full name of the owner. */
        std::uint8_t age = 0; /**< Age of the owner in years. */
    };

    /**
     * @brief Seating and payload capacity specs.
     */
    struct Capacity
    {
        std::uint8_t seatsNumber = 0; /**< Number of passenger seats available. */
        float maxPayloadKg = 0.0f;    /**< Maximum allowed cargo weight in kilograms. */
    };

    /**
     * @brief Manufacturing metadata for vehicles.
     */
    struct Specification
    {
        std::uint16_t modelYear = 2026; /**< Year of manufacture. */
        std::string modelName = "";     /**< Model or brand name string. */
    };
}