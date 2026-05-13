/*
** EPITECH PROJECT, 2026
** test_plane
** File description:
** Unit tests for Plane primitive
*/

#include <gtest/gtest.h>

#include "libs/primitive/plane/Plane.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::Plane;

TEST(PRIMITIVE, plane_constructor_normalizes) {
    Plane plane(Vector(0, 0, 0), Vector(0, 2, 0));
    const auto &normal = plane.normal();
    ASSERT_DOUBLE_EQ(normal.x, 0.0);
    ASSERT_DOUBLE_EQ(normal.y, 1.0);
    ASSERT_DOUBLE_EQ(normal.z, 0.0);
}

TEST(PRIMITIVE, plane_hits_front_face) {
    Plane plane(Vector(0, 0, 0), Vector(0, 1, 0));
    Ray ray(Vector(0, -1, 0), Vector(0, 1, 0));

    double t = plane.hits(ray);
    ASSERT_DOUBLE_EQ(t, 1.0);
}

TEST(PRIMITIVE, plane_parallel_no_hit) {
    Plane plane(Vector(0, 0, 0), Vector(0, 1, 0));
    Ray ray(Vector(0, -1, 0), Vector(1, 0, 0));

    double t = plane.hits(ray);
    ASSERT_DOUBLE_EQ(t, -1.0);
}

TEST(PRIMITIVE, plane_surface_data_uv_and_normal) {
    Plane plane(Vector(0, 0, 0), Vector(0, 1, 0));
    Vector hitPoint(1, 0, 0);

    auto data = plane.surfaceData(
        raytracer::object::primitive::HitRecord{0, -1, 0, hitPoint});

    ASSERT_DOUBLE_EQ(data.normal.x, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.y, 1.0);
    ASSERT_DOUBLE_EQ(data.normal.z, 0.0);

    // For normal (0,1,0), helper is (1,0,0); uAxis = (0,0,-1), vAxis = (-1,0,0)
    ASSERT_DOUBLE_EQ(data.uv.x, 0.0);
    ASSERT_DOUBLE_EQ(data.uv.y, -1.0);
}
