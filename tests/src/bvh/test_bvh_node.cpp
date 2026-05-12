/*
** EPITECH PROJECT, 2026
** test_bvh_node
** File description:
** Unit tests for BVHNode
*/

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "bvh/BVHNode.hpp"
#include "exception/CoreException.hpp"
#include "math/Ray.hpp"
#include "tests/src/bvh/bvh_test_helpers.hpp"

using raytracer::bvh::BVHNode;
using raytracer::maths::AABoundingBox;
using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::tests::bvh::makePrimitive;

namespace {
    std::shared_ptr<raytracer::object::primitive::IPrimitive> makeLeafPrimitive(
        const std::string &name, const AABoundingBox &box, const Vector &center,
        double hitT, int id) {
        return makePrimitive(name, box, center, hitT, id);
    }
}  // namespace

TEST(BVH_NODE, leaf_returns_closest_hit) {
    std::vector<std::shared_ptr<raytracer::object::primitive::IPrimitive>>
        primitives = {
            makeLeafPrimitive("far", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5),
                              4.0, 0),
            makeLeafPrimitive("near", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5),
                              2.0, 1),
            makeLeafPrimitive("mid", {0, 0, 0, 1, 1, 1}, Vector(0.5, 0.5, 0.5),
                              3.0, 2),
        };
    BVHNode node({0, 0, 0, 1, 1, 1}, primitives);
    Ray ray(Vector(0.5, 0.5, -1), Vector(0, 0, 1));

    raytracer::object::primitive::HitRecord record;

    ASSERT_TRUE(node.hits(ray, record));
    ASSERT_DOUBLE_EQ(record.t, 2.0);
    ASSERT_EQ(record.objectId, 1);
}

TEST(BVH_NODE, internal_returns_closest_child_hit) {
    auto left = makeLeafPrimitive("left", {0, 0, 0, 1, 1, 1},
                                  Vector(0.5, 0.5, 0.5), 3.0, 10);
    auto right = makeLeafPrimitive("right", {0, 0, 0, 1, 1, 1},
                                   Vector(0.5, 0.5, 0.5), 1.0, 11);
    BVHNode node({0, 0, 0, 1, 1, 1}, left, right);
    Ray ray(Vector(0.5, 0.5, -1), Vector(0, 0, 1));

    raytracer::object::primitive::HitRecord record;

    ASSERT_TRUE(node.hits(ray, record));
    ASSERT_DOUBLE_EQ(record.t, 1.0);
    ASSERT_EQ(record.objectId, 11);
}

TEST(BVH_NODE, bbox_miss_returns_false_and_scalar_hit_is_minus_one) {
    auto primitive = makeLeafPrimitive("single", {0, 0, 0, 1, 1, 1},
                                       Vector(0.5, 0.5, 0.5), 1.0, 5);
    BVHNode node(
        {0, 0, 0, 1, 1, 1},
        std::vector<std::shared_ptr<raytracer::object::primitive::IPrimitive>>{
            primitive});
    Ray ray(Vector(5, 5, 5), Vector(1, 0, 0));

    raytracer::object::primitive::HitRecord record;

    ASSERT_FALSE(node.hits(ray, record));
    const auto &asPrimitive =
        static_cast<const raytracer::object::primitive::IPrimitive &>(node);
    ASSERT_DOUBLE_EQ(asPrimitive.hits(ray), -1.0);
}

TEST(BVH_NODE, exposes_bbox_center_and_name) {
    BVHNode node(
        {2, 4, 6, 8, 10, 12},
        std::vector<
            std::shared_ptr<raytracer::object::primitive::IPrimitive>>{});

    const auto box = node.boundingBox();
    const auto center = node.center();

    ASSERT_EQ(node.name(), "BVHNode");
    ASSERT_DOUBLE_EQ(box.x, 2.0);
    ASSERT_DOUBLE_EQ(box.y, 4.0);
    ASSERT_DOUBLE_EQ(box.z, 6.0);
    ASSERT_DOUBLE_EQ(box.w, 8.0);
    ASSERT_DOUBLE_EQ(box.h, 10.0);
    ASSERT_DOUBLE_EQ(box.d, 12.0);
    ASSERT_DOUBLE_EQ(center.x, 6.0);
    ASSERT_DOUBLE_EQ(center.y, 9.0);
    ASSERT_DOUBLE_EQ(center.z, 12.0);
}

TEST(BVH_NODE, surface_data_throws) {
    BVHNode node(
        {0, 0, 0, 1, 1, 1},
        std::vector<
            std::shared_ptr<raytracer::object::primitive::IPrimitive>>{});

    ASSERT_THROW(node.surfaceData(Vector(0, 0, 0)),
                 raytracer::exception::CoreException);
}