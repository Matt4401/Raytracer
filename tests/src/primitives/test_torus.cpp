/*
** EPITECH PROJECT, 2026
** test_torus
** File description:
** Unit tests for Torus primitive
*/

#include <gtest/gtest.h>

#include <cmath>

#include "exception/PluginException.hpp"
#include "libs/primitive/torus/Torus.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::HitRecord;
using raytracer::object::primitive::Torus;

TEST(PRIMITIVE, torus_constructor_stores_radii) {
    Torus torus(Vector(0, 0, 0), 2.5, 1.25);

    ASSERT_DOUBLE_EQ(torus.majorRadius(), 2.5);
    ASSERT_DOUBLE_EQ(torus.minorRadius(), 1.25);
}

TEST(PRIMITIVE, torus_hits_from_outside_on_major_axis) {
    Torus torus(Vector(0, 0, 0), 2.0, 1.0);
    torus.setId(7);
    Ray ray(Vector(5, 0, 0), Vector(-1, 0, 0));
    HitRecord record;

    const bool hit = torus.hits(ray, record);

    ASSERT_TRUE(hit);
    ASSERT_DOUBLE_EQ(record.t, 2.0);
    ASSERT_DOUBLE_EQ(record.hitPoint.x, 3.0);
    ASSERT_DOUBLE_EQ(record.hitPoint.y, 0.0);
    ASSERT_DOUBLE_EQ(record.hitPoint.z, 0.0);
    ASSERT_EQ(record.objectId, 7);
    ASSERT_DOUBLE_EQ(torus.hits(ray), 2.0);
}

TEST(PRIMITIVE, torus_miss_returns_negative_one) {
    Torus torus(Vector(0, 0, 0), 2.0, 1.0);
    Ray ray(Vector(0, 0, -5), Vector(0, 1, 0));
    HitRecord record;

    const bool hit = torus.hits(ray, record);

    ASSERT_FALSE(hit);
    ASSERT_DOUBLE_EQ(torus.hits(ray), -1.0);
    ASSERT_DOUBLE_EQ(record.t, -1.0);
    ASSERT_EQ(record.objectId, -1);
}

TEST(PRIMITIVE, torus_negative_radius_throws) {
    EXPECT_THROW((void)Torus(Vector(0, 0, 0), -2.0, 1.0),
                 raytracer::exception::PluginException);
}

TEST(PRIMITIVE, torus_surface_data_uses_outer_normal_and_uv) {
    Torus torus(Vector(0, 0, 0), 2.0, 1.0);
    const auto data = torus.surfaceData(HitRecord{0, -1, 0, Vector(3, 0, 0)});

    ASSERT_DOUBLE_EQ(data.normal.x, 1.0);
    ASSERT_DOUBLE_EQ(data.normal.y, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.z, 0.0);
    ASSERT_DOUBLE_EQ(data.uv.x, 0.5);
    ASSERT_DOUBLE_EQ(data.uv.y, 0.5);
}

TEST(PRIMITIVE, torus_bounding_box_is_centered_on_torus) {
    Torus torus(Vector(1, 2, 3), 2.0, 1.0);

    const auto box = torus.boundingBox();

    ASSERT_DOUBLE_EQ(box.x, -2.1);
    ASSERT_DOUBLE_EQ(box.y, 0.9);
    ASSERT_NEAR(box.z, -0.1, 1e-12);
    ASSERT_DOUBLE_EQ(box.w, 6.2);
    ASSERT_DOUBLE_EQ(box.h, 2.2);
    ASSERT_DOUBLE_EQ(box.d, 6.2);
}