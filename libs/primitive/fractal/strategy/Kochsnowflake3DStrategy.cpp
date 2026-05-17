/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** KochSnowflake3DStrategy
*/

#include "Kochsnowflake3DStrategy.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::primitive::fractal {
    KochSnowflake3DStrategy::KochSnowflake3DStrategy(
        const int iterations, const double scale, const double offset) noexcept
        : _iterations(iterations), _scale(scale), _offset(offset) {
    }

    double KochSnowflake3DStrategy::distanceEstimate(
        const maths::Vector &p) const noexcept {
        maths::Vector z = p;
        double accumScale = 1.0;
        const double trans = _offset * (_scale - 1.0);

        for (int n = 0; n < _iterations; ++n) {
            if (z.x + z.y < 0.0) {
                const double tmp = z.x;
                z.x = -z.y;
                z.y = -tmp;
            }
            if (z.x + z.z < 0.0) {
                const double tmp = z.x;
                z.x = -z.z;
                z.z = -tmp;
            }
            if (z.y + z.z < 0.0) {
                const double tmp = z.y;
                z.y = -z.z;
                z.z = -tmp;
            }
            z.x = _scale * z.x - trans;
            z.y = _scale * z.y - trans;
            z.z = _scale * z.z - trans;
            accumScale *= _scale;
        }
        return (z.magnitude() - std::sqrt(3.0)) / accumScale;
    }

    double KochSnowflake3DStrategy::boundingRadius() const noexcept {
        return LOG_SCALE;
    }
}  // namespace raytracer::object::primitive::fractal
