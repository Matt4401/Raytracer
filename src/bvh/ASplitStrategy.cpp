/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ASplitStrategy
*/

#include "bvh/ASplitStrategy.hpp"

namespace raytracer::bvh {
    Axis ASplitStrategy::longestAxis(const maths::AABoundingBox& nodeBox) {
        if (nodeBox.w >= nodeBox.h && nodeBox.w >= nodeBox.d)
            return Axis::X;
        if (nodeBox.h >= nodeBox.w && nodeBox.h >= nodeBox.d)
            return Axis::Y;
        return Axis::Z;
    }

    bool ASplitStrategy::hasEnoughPrimitives(const std::size_t count) {
        return count > MIN_PRIMITIVES_FOR_SPLIT;
    }
}  // namespace raytracer::bvh