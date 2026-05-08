/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ASplitStrategy
*/

#pragma once

#include <cstddef>

#include "bvh/ISplitStrategy.hpp"
#include "math/AABoundingBox.hpp"

namespace raytracer::bvh {
    class ASplitStrategy : public ISplitStrategy {
      protected:
        static constexpr std::size_t MIN_PRIMITIVES_FOR_SPLIT = 2;

        static Axis longestAxis(const maths::AABoundingBox& nodeBox);
        static bool hasEnoughPrimitives(std::size_t count);
    };
}  // namespace raytracer::bvh
