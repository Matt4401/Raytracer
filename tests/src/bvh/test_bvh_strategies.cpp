/*
** EPITECH PROJECT, 2026
** test_bvh_strategies
** File description:
** Unit tests for split strategies
*/

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "bvh/bvhSplitStrategy/MedianStrategy.hpp"
#include "bvh/bvhSplitStrategy/MidpointStrategy.hpp"
#include "bvh/bvhSplitStrategy/SAHStrategy.hpp"
#include "tests/src/bvh/bvh_test_helpers.hpp"

using raytracer::maths::AABoundingBox;
using raytracer::maths::Vector;
using raytracer::tests::bvh::ExposedASplitStrategy;
using raytracer::tests::bvh::makePrimitive;

namespace {
    using PrimitivePtr = std::shared_ptr<raytracer::object::primitive::IPrimitive>;

    std::vector<PrimitivePtr> makeXAxisPrimitives(std::size_t count) {
        std::vector<PrimitivePtr> primitives;

        for (std::size_t i = 0; i < count; ++i) {
            const double x = static_cast<double>(i) * 2.0;
            primitives.push_back(
                makePrimitive("primitive" + std::to_string(i), {x, 0, 0, 1, 1, 1},
                              Vector(x + 0.5, 0.5, 0.5), 1.0));
        }
        return primitives;
    }
}  // namespace

TEST(BVH_STRATEGY_HELPERS, longest_axis_prefers_x_then_y_then_z) {
    ASSERT_EQ(ExposedASplitStrategy::longestAxis({0, 0, 0, 4, 2, 1}), raytracer::bvh::Axis::X);
    ASSERT_EQ(ExposedASplitStrategy::longestAxis({0, 0, 0, 1, 4, 2}), raytracer::bvh::Axis::Y);
    ASSERT_EQ(ExposedASplitStrategy::longestAxis({0, 0, 0, 1, 2, 4}), raytracer::bvh::Axis::Z);
    ASSERT_EQ(ExposedASplitStrategy::longestAxis({0, 0, 0, 3, 3, 3}), raytracer::bvh::Axis::X);
}

TEST(BVH_STRATEGY_HELPERS, split_threshold_requires_more_than_four_objects) {
    ASSERT_FALSE(ExposedASplitStrategy::hasEnoughPrimitives(4));
    ASSERT_TRUE(ExposedASplitStrategy::hasEnoughPrimitives(5));
}

TEST(BVH_STRATEGIES, median_uses_median_center_on_longest_axis) {
    raytracer::bvh::MedianStrategy strategy;
    const auto primitives = makeXAxisPrimitives(5);
    const AABoundingBox box{0, 0, 0, 10, 1, 1};

    const auto result = strategy.findSplit(primitives, box);

    ASSERT_TRUE(result.shouldSplit);
    ASSERT_EQ(result.axis, raytracer::bvh::Axis::X);
    ASSERT_DOUBLE_EQ(result.splitPos, 4.5);
}

TEST(BVH_STRATEGIES, median_refuses_to_split_small_sets) {
    raytracer::bvh::MedianStrategy strategy;
    const auto primitives = makeXAxisPrimitives(4);
    const AABoundingBox box{0, 0, 0, 10, 1, 1};

    const auto result = strategy.findSplit(primitives, box);

    ASSERT_FALSE(result.shouldSplit);
    ASSERT_DOUBLE_EQ(result.splitPos, 0.0);
    ASSERT_EQ(result.axis, raytracer::bvh::Axis::X);
}

TEST(BVH_STRATEGIES, midpoint_splits_box_in_half) {
    raytracer::bvh::MidpointStrategy strategy;
    const auto primitives = makeXAxisPrimitives(5);
    const AABoundingBox box{0, 0, 0, 10, 1, 1};

    const auto result = strategy.findSplit(primitives, box);

    ASSERT_TRUE(result.shouldSplit);
    ASSERT_EQ(result.axis, raytracer::bvh::Axis::X);
    ASSERT_DOUBLE_EQ(result.splitPos, 5.0);
}

TEST(BVH_STRATEGIES, sah_rejects_degenerate_boxes) {
    raytracer::bvh::SAHStrategy strategy;
    const auto primitives = makeXAxisPrimitives(5);
    const AABoundingBox box{0, 0, 0, 0, 0, 0};

    const auto result = strategy.findSplit(primitives, box);

    ASSERT_FALSE(result.shouldSplit);
    ASSERT_DOUBLE_EQ(result.splitPos, 0.0);
}

TEST(BVH_STRATEGIES, sah_splits_non_degenerate_scene) {
    raytracer::bvh::SAHStrategy strategy;
    const auto primitives = makeXAxisPrimitives(5);
    const AABoundingBox box{0, 0, 0, 10, 1, 1};

    const auto result = strategy.findSplit(primitives, box);

    ASSERT_TRUE(result.shouldSplit);
    ASSERT_EQ(result.axis, raytracer::bvh::Axis::X);
    ASSERT_GT(result.splitPos, 0.0);
    ASSERT_LT(result.splitPos, 10.0);
}