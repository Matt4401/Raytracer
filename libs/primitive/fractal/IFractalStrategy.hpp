/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IFractalStrategy
*/

#pragma once

#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    class IFractalStrategy {
      public:
        virtual ~IFractalStrategy() = default;

        /**
         * This method estimates the distance from a point in space to the
         * fractal surface. It is used in ray marching algorithms to determine
         * how far along a ray we can safely march without missing the surface.
         * @param p The point in space from which to estimate the distance to
         * the fractal surface.
         * @return An estimate of the distance from point p to the nearest point
         * on the fractal surface. A smaller value indicates that p is closer to
         * the surface, while a larger value indicates that p is farther away.
         */
        [[nodiscard]] virtual double distanceEstimate(
            const maths::Vector &p) const noexcept = 0;

        /**
         * This method returns the bounding radius of the fractal. The bounding
         * radius is the maximum distance from the fractal's center to any point
         * on its surface. It is used to optimize ray marching by providing an
         * upper bound on how far rays can be marched before they are guaranteed
         * to miss the fractal. If a ray is farther than this radius from the
         * fractal's center, it can be safely ignored in intersection tests.
         * @return The bounding radius of the fractal, which is a positive value
         * representing the maximum distance from the fractal's center to its
         * surface. A smaller value indicates a more compact fractal, while a
         * larger value indicates a more expansive fractal.
         */
        [[nodiscard]] virtual double boundingRadius() const noexcept = 0;
    };
}  // namespace raytracer::object::primitive
