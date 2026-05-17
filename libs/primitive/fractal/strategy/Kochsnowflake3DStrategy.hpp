/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** KochSnowflake3DStrategy
*/

#pragma once

#include "fractal/IFractalStrategy.hpp"

namespace raytracer::object::primitive::fractal {
    class KochSnowflake3DStrategy : public IFractalStrategy {
      public:
        static constexpr int DEFAULT_ITERATIONS = 8;
        static constexpr double DEFAULT_SCALE = 3.0;
        static constexpr double DEFAULT_OFFSET = 1.0;

        explicit KochSnowflake3DStrategy(
            int iterations = DEFAULT_ITERATIONS, double scale = DEFAULT_SCALE,
            double offset = DEFAULT_OFFSET) noexcept;

        ~KochSnowflake3DStrategy() override = default;

        [[nodiscard]] double distanceEstimate(
            const maths::Vector &p) const noexcept override;
        [[nodiscard]] double boundingRadius() const noexcept override;

      private:
        int _iterations;
        double _scale;
        double _offset;

        static constexpr double LOG_SCALE = 2.0;
    };

}  // namespace raytracer::object::primitive::fractal