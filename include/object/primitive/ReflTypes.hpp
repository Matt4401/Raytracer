/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ReflTypes
*/

#pragma once

#include <cstdint>

namespace raytracer::object::primitive {
    // This enum class defines the different types of reflection :
    // - DIFF: This type of reflection represents diffuse reflection, where
    // light is scattered in many directions after hitting the surface of the
    // object.
    // - SPEC: This type of reflection represents specular reflection, where
    // light is reflected in a single direction after hitting the surface of the
    // object.
    // - REFR: This type of reflection represents refractive reflection, where
    // light is bent as it passes through the surface of the object.
    enum class RefltT : std::uint8_t {
        DIFF,
        SPEC,
        REFR,
    };
}  // namespace raytracer::object::primitive
