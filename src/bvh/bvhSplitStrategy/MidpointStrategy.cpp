/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MidpointStrategy
*/

#include "bvh/bvhSplitStrategy/MidpointStrategy.hpp"

namespace raytracer::bvh {
    SplitResult MidpointStrategy::findSplit(
        const PrimitiveVec &objs, const maths::AABoundingBox &nodeBox) {
        SplitResult res;

        res.axis = longestAxis(nodeBox);
        if (res.axis == Axis::X)
            res.splitPos = nodeBox.x + (nodeBox.w / 2.0);
        else if (res.axis == Axis::Y)
            res.splitPos = nodeBox.y + (nodeBox.h / 2.0);
        else
            res.splitPos = nodeBox.z + (nodeBox.d / 2.0);
        res.shouldSplit = hasEnoughPrimitives(objs.size());
        if (!res.shouldSplit)
            res.splitPos = 0;
        return res;
    }
}  // namespace raytracer::bvh
