/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MandelbulbStrategy
*/

#pragma once

#include "fractal/IFractalStrategy.hpp"

namespace raytracer::object::primitive {
    class MandelbulbStrategy : public IFractalStrategy {
      public:
        static constexpr double DEFAULT_POWER = 8.0;
        static constexpr int DEFAULT_ITERATIONS = 100;
        static constexpr double DEFAULT_BAILOUT = 2.0;

        explicit MandelbulbStrategy(double power = DEFAULT_POWER,
                                    int iterations = DEFAULT_ITERATIONS,
                                    double bailout = DEFAULT_BAILOUT) noexcept;

        ~MandelbulbStrategy() override = default;

        [[nodiscard]] double distanceEstimate(
            const maths::Vector &p) const noexcept override;

        [[nodiscard]] double boundingRadius() const noexcept override;

      private:
        double _power;
        int _iterations;
        double _bailout;

        static constexpr double MIN_DERIVATIVE_EPSILON = 1e-15;
    };

}  // namespace raytracer::object::primitive