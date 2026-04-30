/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** IMaterial
*/

#pragma once

#include "object/primitive/IPrimitive.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::material {
    class IMaterial {
      public:
        virtual ~IMaterial() = default;

        virtual void apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const = 0;
    };
}
