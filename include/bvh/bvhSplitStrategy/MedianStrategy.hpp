/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MedianStrategy
*/

#pragma once

#include <bvh/BVHBuilder.hpp>
#include <bvh/ISplitStrategy.hpp>

namespace raytracer::bvh {
    class MedianStrategy : public ISplitStrategy {
      public:
        MedianStrategy() = default;
        ~MedianStrategy() override = default;

        SplitResult findSplit(const PrimitiveVec &objs,
                              const maths::AABoundingBox &nodeBox) override;
    };
}  // namespace raytracer::bvh