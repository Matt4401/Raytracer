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

        if (nodeBox.w >= nodeBox.h && nodeBox.w >= nodeBox.d)
            res.axis = Axis::X;
        else if (nodeBox.h >= nodeBox.w && nodeBox.h >= nodeBox.d)
            res.axis = Axis::Y;
        else
            res.axis = Axis::Z;
        if (res.axis == Axis::X)
            res.splitPos = nodeBox.x + (nodeBox.w / 2.0);
        else if (res.axis == Axis::Y)
            res.splitPos = nodeBox.y + (nodeBox.h / 2.0);
        else
            res.splitPos = nodeBox.z + (nodeBox.d / 2.0);
        res.shouldSplit = (objs.size() > 2);
        return res;
    }
}  // namespace raytracer::bvh
