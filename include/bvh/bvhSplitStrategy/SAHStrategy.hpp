/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SAH
*/

#pragma once

#include <bvh/ASplitStrategy.hpp>
#include <bvh/BVHBuilder.hpp>

#include "math/AABoundingBox.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    class SAHStrategy : public ASplitStrategy {
      public:
        struct Bucket {
            size_t count = 0;
            maths::AABoundingBox bbox;
        };

        struct BestSplit {
            double minCost;
            size_t bestSplitBucket;
        };

        SplitResult findSplit(const PrimitiveVec& objs,
                              const maths::AABoundingBox& nodeBox) override;

      private:
        static constexpr size_t N_BUCKETS = 12;
        static constexpr double C_TRAV = 1.0;

        static std::vector<Bucket> buckets(const SplitResult& res,
                                           const PrimitiveVec& objs,
                                           const maths::AABoundingBox& nodeBox);
        static BestSplit computeCost(const std::vector<Bucket>& buckets,
                                     const maths::AABoundingBox& nodeBox);
    };
}  // namespace raytracer::bvh