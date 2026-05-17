/*
** EPITECH PROJECT, 2026
** test_tanglecube
** File description:
** Unit tests for TangleCube primitive
*/

#include <gtest/gtest.h>

#include <cmath>

#include "libs/primitive/tanglecube/TangleCube.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::HitRecord;
using raytracer::object::primitive::TangleCube;

TEST(PRIMITIVE, tanglecube_constructor_stores_constant) {
    TangleCube cube(Vector(0, 0, 0), 3.5);

    ASSERT_DOUBLE_EQ(cube.constant(), 3.5);
}

TEST(PRIMITIVE, tanglecube_hits_from_outside_on_axis) {
    TangleCube cube(Vector(0, 0, 0), 0.0);
    cube.setId(42);
    Ray ray(Vector(5, 0, 0), Vector(-1, 0, 0));
    HitRecord record;

    const bool hit = cube.hits(ray, record);

    ASSERT_TRUE(hit);
    ASSERT_NEAR(record.t, 5.0 - std::sqrt(5.0), 1e-12);
    ASSERT_DOUBLE_EQ(record.hitPoint.x, std::sqrt(5.0));
    ASSERT_DOUBLE_EQ(record.hitPoint.y, 0.0);
    ASSERT_DOUBLE_EQ(record.hitPoint.z, 0.0);
    ASSERT_EQ(record.objectId, 42);
    ASSERT_NEAR(cube.hits(ray), 5.0 - std::sqrt(5.0), 1e-12);
}

TEST(PRIMITIVE, tanglecube_miss_returns_negative_one) {
    TangleCube cube(Vector(0, 0, 0), 0.0);
    Ray ray(Vector(5, 3, 0), Vector(-1, 0, 0));
    HitRecord record;

    const bool hit = cube.hits(ray, record);

    ASSERT_FALSE(hit);
    ASSERT_DOUBLE_EQ(cube.hits(ray), -1.0);
    ASSERT_DOUBLE_EQ(record.t, -1.0);
    ASSERT_EQ(record.objectId, -1);
}

TEST(PRIMITIVE, tanglecube_surface_data_uses_normal_and_uv) {
    TangleCube cube(Vector(0, 0, 0), 0.0);
    const auto data =
        cube.surfaceData(HitRecord{0, -1, 0, Vector(std::sqrt(5.0), 0.0, 0.0)});

    ASSERT_DOUBLE_EQ(data.normal.x, 1.0);
    ASSERT_DOUBLE_EQ(data.normal.y, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.z, 0.0);
    ASSERT_DOUBLE_EQ(data.uv.x, 0.5);
    ASSERT_DOUBLE_EQ(data.uv.y, 0.5);
}

TEST(PRIMITIVE, tanglecube_bounding_box_is_centered_on_constant) {
    TangleCube cube(Vector(1, 2, 3), 0.0);

    const auto box = cube.boundingBox();
    const double extent = std::sqrt((5.0 + std::sqrt(75.0)) / 2.0) * 1.05;

    ASSERT_NEAR(box.x, 1.0 - extent, 1e-12);
    ASSERT_NEAR(box.y, 2.0 - extent, 1e-12);
    ASSERT_NEAR(box.z, 3.0 - extent, 1e-12);
    ASSERT_NEAR(box.w, extent * 2.0, 1e-12);
    ASSERT_NEAR(box.h, extent * 2.0, 1e-12);
    ASSERT_NEAR(box.d, extent * 2.0, 1e-12);
}