/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ISplitStrategy
*/

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "math/AABoundingBox.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    enum class Axis : std::uint8_t { X = 0, Y = 1, Z = 2 };

    struct SplitResult {
        Axis axis;
        double splitPos;
        bool shouldSplit;
    };

    class ISplitStrategy {
      public:
        virtual ~ISplitStrategy() = default;

        /**
         * This function is responsible for determining the optimal way to split
         * a set of primitives within a bounding volume hierarchy (BVH) node.
         * The function takes in a vector of shared pointers to primitive
         * objects and the axis-aligned bounding box (AABB) that represents the
         * spatial extent of the nodes.
         * @param objs a vector of shared pointers to primitive objects that are
         * contained within the BVH node. Each primitive object represents a
         * geometric shape or entity in the scene, such as a triangle, sphere,
         * or other 3D object. The function will analyze these primitives to
         * determine how best to split them for efficient ray tracing.
         * @param nodeBox the axis-aligned bounding box (AABB) that represents
         * the spatial extent of the BVH node. This bounding box is defined by
         * its minimum and maximum corners in 3D space and is used to determine
         * how to split the primitives based on their spatial distribution
         * within the node.
         * @return a SplitResult structure that contains information about the
         * chosen split, including the axis along which to split, the position
         * of the split, and whether the split should be performed. The
         * SplitResult will guide the BVH construction process in determining
         * how to partition the primitives for optimal ray tracing performance.
         */
        virtual SplitResult findSplit(
            const std::vector<std::shared_ptr<object::primitive::IPrimitive>>&
                objs,
            const maths::AABoundingBox& nodeBox) = 0;
    };
}  // namespace raytracer::bvh
