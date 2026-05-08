/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ISplitStrategy
*/

#pragma once

#include <memory>
#include <vector>

#include "math/AABoundingBox.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    struct SplitResult {
        int axis;
        double splitPos;
        bool shouldSplit;
    };

    class ISplitStrategy {
      public:
        virtual ~ISplitStrategy() = default;
        virtual SplitResult findSplit(
            const std::vector<std::shared_ptr<object::primitive::IPrimitive>>&
                objs,
            const maths::AABoundingBox& nodeBox) = 0;
    };
}  // namespace raytracer::bvh
