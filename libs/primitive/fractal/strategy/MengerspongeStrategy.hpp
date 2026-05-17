/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MengerSpongeStrategy
*/

#pragma once

#include "fractal/IFractalStrategy.hpp"

namespace raytracer::object::primitive::fractal {
    class MengerSpongeStrategy : public IFractalStrategy {
      public:
        static constexpr int DEFAULT_ITERATIONS = 4;

        explicit MengerSpongeStrategy(
            int iterations = DEFAULT_ITERATIONS) noexcept;

        ~MengerSpongeStrategy() override = default;

        [[nodiscard]] double distanceEstimate(
            const maths::Vector &p) const noexcept override;
        [[nodiscard]] double boundingRadius() const noexcept override;

      private:
        int _iterations;
    };

}  // namespace raytracer::object::primitive::fractal