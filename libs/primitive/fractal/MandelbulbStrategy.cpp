/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MandelbulbStrategy
*/

#include "fractal/MandelbulbStrategy.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::primitive {
    MandelbulbStrategy::MandelbulbStrategy(const double power,
                                           const int iterations,
                                           const double bailout) noexcept
        : _power(power), _iterations(iterations), _bailout(bailout) {
    }

    double MandelbulbStrategy::distanceEstimate(
        const maths::Vector &p) const noexcept {
        maths::Vector z = p;
        double dr = 1.0;
        double r = 0.0;

        for (int i = 0; i < _iterations; ++i) {
            r = z.magnitude();
            if (r > _bailout)
                break;

            const double theta = std::acos(std::clamp(z.y / r, -1.0, 1.0));
            const double phi = std::atan2(z.z, z.x);

            dr = std::pow(r, _power - 1.0) * _power * dr + 1.0;

            const double rn = std::pow(r, _power);
            const double thetan = theta * _power;
            const double phin = phi * _power;
            const double sinTheta = std::sin(thetan);

            z = maths::Vector(sinTheta * std::cos(phin), std::cos(thetan),
                              sinTheta * std::sin(phin)) *
                rn;
            z = z + p;
        }
        if (dr < 1e-15)
            return 0.0;
        return 0.5 * std::log(r) * r / dr;
    }

    double MandelbulbStrategy::boundingRadius() const noexcept {
        return _bailout * 1.2;
    }

}  // namespace raytracer::object::primitive