/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SAH
*/

#include "bvh/bvhSplitStrategy/SAHStrategy.hpp"

namespace raytracer::bvh {
    std::vector<SAHStrategy::Bucket> SAHStrategy::buckets(
        const SplitResult& res, const PrimitiveVec& objs,
        const maths::AABoundingBox& nodeBox) {
        std::vector<Bucket> buckets(N_BUCKETS);

        for (const auto& obj : objs) {
            const double c = (res.axis == Axis::X)   ? obj->center().x
                             : (res.axis == Axis::Y) ? obj->center().y
                                                     : obj->center().z;
            const double offset =
                (res.axis == Axis::X)   ? (c - nodeBox.x) / nodeBox.w
                : (res.axis == Axis::Y) ? (c - nodeBox.y) / nodeBox.h
                                        : (c - nodeBox.z) / nodeBox.d;
            size_t b = static_cast<size_t>(N_BUCKETS * offset);

            if (b >= N_BUCKETS)
                b = N_BUCKETS - 1;
            buckets[b].count++;
            buckets[b].bbox.extend(obj->boundingBox());
        }
        return buckets;
    }

    SAHStrategy::BestSplit SAHStrategy::computeCost(
        const std::vector<Bucket>& buckets,
        const maths::AABoundingBox& nodeBox) {
        BestSplit bestSplit{std::numeric_limits<double>::infinity(), 0};

        for (size_t i = 0; i < N_BUCKETS - 1; ++i) {
            maths::AABoundingBox boxA, boxB;
            size_t countA = 0, countB = 0;

            for (size_t j = 0; j <= i; ++j) {
                boxA.extend(buckets[j].bbox);
                countA += buckets[j].count;
            }
            for (size_t j = i + 1; j < N_BUCKETS; ++j) {
                boxB.extend(buckets[j].bbox);
                countB += buckets[j].count;
            }
            const double cost = C_TRAV + (boxA.surfaceData() * countA +
                                          boxB.surfaceData() * countB) /
                                             nodeBox.surfaceData();
            if (cost < bestSplit.minCost) {
                bestSplit.minCost = cost;
                bestSplit.bestSplitBucket = i;
            }
        }
        return bestSplit;
    }

    SplitResult SAHStrategy::findSplit(const PrimitiveVec& objs,
                                       const maths::AABoundingBox& nodeBox) {
        SplitResult res;
        const size_t nObjs = objs.size();

        if (nodeBox.w >= nodeBox.h && nodeBox.w >= nodeBox.d)
            res.axis = Axis::X;
        else if (nodeBox.h >= nodeBox.w && nodeBox.h >= nodeBox.d)
            res.axis = Axis::Y;
        else
            res.axis = Axis::Z;

        if (nObjs <= 2) {
            res.shouldSplit = false;
            res.splitPos = 0;
            return res;
        }

        const auto buckets = SAHStrategy::buckets(res, objs, nodeBox);
        const auto [minCost, bestSplitBucket] = computeCost(buckets, nodeBox);

        if (const double leafCost = static_cast<double>(nObjs);
            minCost < leafCost) {
            res.shouldSplit = true;
            const double step = (res.axis == Axis::X)   ? nodeBox.w
                                : (res.axis == Axis::Y) ? nodeBox.h
                                                        : nodeBox.d;
            res.splitPos = ((res.axis == Axis::X)   ? nodeBox.x
                            : (res.axis == Axis::Y) ? nodeBox.y
                                                    : nodeBox.z) +
                           (step * (bestSplitBucket + 1) / N_BUCKETS);
        } else {
            res.splitPos = 0;
            res.shouldSplit = false;
        }
        return res;
    }
}  // namespace raytracer::bvh
