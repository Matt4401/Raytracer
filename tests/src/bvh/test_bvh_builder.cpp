/*
** EPITECH PROJECT, 2026
** test_bvh_builder
** File description:
** Unit tests for BVHBuilder
*/

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "bvh/BVHBuilder.hpp"
#include "bvh/ISplitStrategy.hpp"
#include "bvh/bvhSplitStrategy/MedianStrategy.hpp"
#include "bvh/bvhSplitStrategy/MidpointStrategy.hpp"
#include "math/Ray.hpp"
#include "tests/src/bvh/bvh_test_helpers.hpp"

using raytracer::maths::AABoundingBox;
using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::tests::bvh::makePrimitive;

namespace {
    using PrimitivePtr = std::shared_ptr<raytracer::object::primitive::IPrimitive>;

    std::vector<PrimitivePtr> buildLinearPrimitives() {
        std::vector<PrimitivePtr> primitives;

        primitives.push_back(makePrimitive("p0", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5), 5.0));
        primitives.push_back(makePrimitive("p1", {2, 0, 0, 1, 1, 1}, Vector(2.5, 0.5, 0.5), 4.0));
        primitives.push_back(makePrimitive("p2", {4, 0, 0, 1, 1, 1}, Vector(4.5, 0.5, 0.5), 3.0));
        primitives.push_back(makePrimitive("p3", {6, 0, 0, 1, 1, 1}, Vector(6.5, 0.5, 0.5), 2.0));
        primitives.push_back(makePrimitive("p4", {8, 0, 0, 1, 1, 1}, Vector(8.5, 0.5, 0.5), 1.0));
        return primitives;
    }
}  // namespace

TEST(BVH_BUILDER, empty_input_returns_missable_node) {
    raytracer::bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("median");

    const auto tree = builder.build({});
    ASSERT_NE(tree, nullptr);

    Ray ray(Vector(0, 0, 0), Vector(1, 0, 0));
    ASSERT_DOUBLE_EQ(tree->hits(ray), -1.0);
}

TEST(BVH_BUILDER, two_objects_stay_in_leaf_logic) {
    raytracer::bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("midpoint");
    std::vector<PrimitivePtr> primitives = {
        makePrimitive("far", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5), 4.0),
        makePrimitive("near", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5), 1.0),
    };

    const auto tree = builder.build(primitives);
    ASSERT_NE(tree, nullptr);

    Ray ray(Vector(0.5, 0.5, -1), Vector(0, 0, 1));
    ASSERT_DOUBLE_EQ(tree->hits(ray), 1.0);
}

TEST(BVH_BUILDER, recursive_build_returns_a_valid_hit) {
    raytracer::bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("median");
    const auto primitives = buildLinearPrimitives();
    const auto tree = builder.build(primitives);

    ASSERT_NE(tree, nullptr);

    Ray ray(Vector(0.5, 0.5, -1), Vector(0, 0, 1));
    ASSERT_DOUBLE_EQ(tree->hits(ray), 4.0);
}

TEST(BVH_BUILDER, midpoint_strategy_handles_same_scene) {
    raytracer::bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("midpoint");
    const auto primitives = buildLinearPrimitives();
    const auto tree = builder.build(primitives);

    ASSERT_NE(tree, nullptr);

    Ray ray(Vector(0.5, 0.5, -1), Vector(0, 0, 1));
    ASSERT_DOUBLE_EQ(tree->hits(ray), 4.0);
}