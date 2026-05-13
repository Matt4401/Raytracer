/*
** EPITECH PROJECT, 2026
** test_rectangle
** File description:
** Unit tests for Rectangle primitive
*/

#include <gtest/gtest.h>

#include "libs/primitive/rectangle/Rectangle.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::HitRecord;
using raytracer::object::primitive::Rectangle;

TEST(PRIMITIVE, rectangle_hits_inside_bounds) {
    Rectangle rectangle(Vector(1, 0, 0), Vector(1, 2, 3), Vector(4, 0, 0));
    rectangle.setId(7);
    Ray ray(Vector(0, 1, 1.5), Vector(1, 0, 0));

    HitRecord record;
    const bool hit = rectangle.hits(ray, record);

    ASSERT_TRUE(hit);
    ASSERT_DOUBLE_EQ(record.t, 1.0);
    ASSERT_EQ(record.objectId, 7);
    ASSERT_DOUBLE_EQ(rectangle.hits(ray), 1.0);
}

TEST(PRIMITIVE, rectangle_miss_outside_bounds) {
    Rectangle rectangle(Vector(1, 0, 0), Vector(1, 2, 3), Vector(0, 0, 1));
    Ray ray(Vector(0, 3, 1.5), Vector(1, 0, 0));

    HitRecord record;
    const bool hit = rectangle.hits(ray, record);

    ASSERT_FALSE(hit);
    ASSERT_DOUBLE_EQ(rectangle.hits(ray), -1.0);
    ASSERT_DOUBLE_EQ(record.t, -1.0);
    ASSERT_EQ(record.objectId, -1);
}

TEST(PRIMITIVE, rectangle_surface_data_uses_normal_and_uv) {
    Rectangle rectangle(Vector(1, 0, 0), Vector(1, 2, 3), Vector(0, 0, 5));

    const auto data = rectangle.surfaceData(raytracer::object::primitive::HitRecord{
        0, -1, 0, raytracer::maths::Vector(0.2, 0.2, 0)});

    ASSERT_DOUBLE_EQ(data.normal.x, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.y, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.z, 1.0);
    ASSERT_DOUBLE_EQ(data.uv.x, 0.0);
    ASSERT_DOUBLE_EQ(data.uv.y, -1.0);
}

TEST(PRIMITIVE, rectangle_bounding_box_expands_thin_axis) {
    Rectangle rectangle(Vector(1, 0, 0), Vector(1, 2, 3), Vector(1, 0, 0));

    const auto box = rectangle.boundingBox();

    ASSERT_NEAR(box.x, 1.0 - 1e-4, 1e-12);
    ASSERT_NEAR(box.w, 2e-4, 1e-12);
    ASSERT_DOUBLE_EQ(box.y, 0.0);
    ASSERT_DOUBLE_EQ(box.h, 2.0);
    ASSERT_DOUBLE_EQ(box.z, 0.0);
    ASSERT_DOUBLE_EQ(box.d, 3.0);
}