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

        virtual raytracer::object::primitive::MaterialProperties evaluate(
            const raytracer::object::primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const = 0;
    };
}  // namespace raytracer::object::material
