/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MidpointStrategy
*/

#pragma once

#include "bvh/ASplitStrategy.hpp"
#include "bvh/BVHBuilder.hpp"

namespace raytracer::bvh {
    class MidpointStrategy : public ASplitStrategy {
      public:
        MidpointStrategy() = default;
        ~MidpointStrategy() override = default;

        SplitResult findSplit(const PrimitiveVec &objs,
                              const maths::AABoundingBox &nodeBox) override;
    };
}  // namespace raytracer::bvh
