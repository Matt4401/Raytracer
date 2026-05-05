/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** IMaterial
*/

#pragma once

#include "math/Vector.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::material {
    class IMaterial {
      public:
        virtual ~IMaterial() = default;

        /**
         * @brief Evaluate the material properties at a given point.
         * @param data The surface data.
         * @param hitPoint The point of intersection.
         * @return The material properties at the given point.
         */
        virtual raytracer::object::primitive::MaterialProperties evaluate(
            const raytracer::object::primitive::SurfaceData& data,
            const maths::Vector& hitPoint) = 0;
    };
}  // namespace raytracer::object::material
