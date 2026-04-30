/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Ray
*/

#pragma once

#include "Vector.hpp"

namespace raytracer::maths {
    class Ray {
      public:
        Ray() : origin(), direction() {};
        Ray(const Vector &origin, const Vector &direction)
            : origin(origin), direction(direction) {};
        ~Ray() = default;

        Vector origin;
        Vector direction;

      protected:
      private:
    };
}  // namespace raytracer::maths
