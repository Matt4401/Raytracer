/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MedianStrategy
*/

#pragma once

#include "bvh/ASplitStrategy.hpp"
#include "bvh/BVHBuilder.hpp"

namespace raytracer::bvh {
    class MedianStrategy : public ASplitStrategy {
      public:
        MedianStrategy() = default;
        ~MedianStrategy() override = default;

        SplitResult findSplit(const PrimitiveVec &objs,
                              const maths::AABoundingBox &nodeBox) override;
    };
}  // namespace raytracer::bvh