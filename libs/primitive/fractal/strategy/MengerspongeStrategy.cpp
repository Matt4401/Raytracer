/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MengerSpongeStrategy
*/

#include "MengerspongeStrategy.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::primitive::fractal {
    MengerSpongeStrategy::MengerSpongeStrategy(const int iterations) noexcept
        : _iterations(iterations) {
    }

    double MengerSpongeStrategy::distanceEstimate(
        const maths::Vector &p) const noexcept {
        auto glslMod = [](const double v, const double m) noexcept -> double {
            return v - m * std::floor(v / m);
        };
        const double qx = std::fabs(p.x) - 1.0;
        const double qy = std::fabs(p.y) - 1.0;
        const double qz = std::fabs(p.z) - 1.0;
        const double outer = std::sqrt(std::max(qx, 0.0) * std::max(qx, 0.0) +
                                       std::max(qy, 0.0) * std::max(qy, 0.0) +
                                       std::max(qz, 0.0) * std::max(qz, 0.0)) +
                             std::min(std::max({qx, qy, qz}), 0.0);
        double d = outer;
        double s = 1.0;

        for (int i = 0; i < _iterations; ++i) {
            const double ax = glslMod(p.x * s, 2.0) - 1.0;
            const double ay = glslMod(p.y * s, 2.0) - 1.0;
            const double az = glslMod(p.z * s, 2.0) - 1.0;
            s *= 3.0;
            const double rx = std::fabs(1.0 - 3.0 * std::fabs(ax));
            const double ry = std::fabs(1.0 - 3.0 * std::fabs(ay));
            const double rz = std::fabs(1.0 - 3.0 * std::fabs(az));
            const double da = std::max(rx, ry);
            const double db = std::max(ry, rz);
            const double dc = std::max(rz, rx);
            const double c = (std::min({da, db, dc}) - 1.0) / s;

            d = std::max(d, c);
        }
        return d;
    }

    double MengerSpongeStrategy::boundingRadius() const noexcept {
        return std::sqrt(3.0) * 1.05;
    }

}  // namespace raytracer::object::primitive::fractal