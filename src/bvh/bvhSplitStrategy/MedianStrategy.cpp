/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MedianStrategy
*/

#include "bvh/bvhSplitStrategy/MedianStrategy.hpp"

namespace raytracer::bvh {
    SplitResult MedianStrategy::findSplit(const PrimitiveVec& objs,
                                          const maths::AABoundingBox& nodeBox) {
        SplitResult res;

        if (nodeBox.w >= nodeBox.h && nodeBox.w >= nodeBox.d)
            res.axis = Axis::X;
        else if (nodeBox.h >= nodeBox.w && nodeBox.h >= nodeBox.d)
            res.axis = Axis::Y;
        else
            res.axis = Axis::Z;
        if (objs.size() <= 2) {
            res.shouldSplit = false;
            res.splitPos = 0;
            return res;
        }

        PrimitiveVec sortedObjs = objs;
        const size_t mid = sortedObjs.size() / 2;

        std::ranges::nth_element(sortedObjs, sortedObjs.begin() + mid,
                                 [res](const auto& a, const auto& b) {
                                     if (res.axis == Axis::X)
                                         return a->center().x < b->center().x;
                                     if (res.axis == Axis::Y)
                                         return a->center().y < b->center().y;
                                     return a->center().z < b->center().z;
                                 });

        const auto medianObj = sortedObjs[mid];

        if (res.axis == Axis::X)
            res.splitPos = medianObj->center().x;
        else if (res.axis == Axis::Y)
            res.splitPos = medianObj->center().y;
        else
            res.splitPos = medianObj->center().z;
        res.shouldSplit = true;
        return res;
    }
}  // namespace raytracer::bvh