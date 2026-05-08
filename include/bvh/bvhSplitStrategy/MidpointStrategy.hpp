/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MidpointStrategy
*/

#pragma once

#include "bvh/BVHBuilder.hpp"
#include "bvh/ISplitStrategy.hpp"

namespace raytracer::bvh {
    class MidpointStrategy : public ISplitStrategy {
      public:
        MidpointStrategy() = default;
        ~MidpointStrategy() override = default;

        SplitResult findSplit(const PrimitiveVec &objs,
                              const maths::AABoundingBox &nodeBox) override;
    };
}  // namespace raytracer::bvh
