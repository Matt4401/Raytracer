/*
** EPITECH PROJECT, 2026
** test_sphere
** File description:
** Unit tests for Sphere primitive
*/

#include <gtest/gtest.h>

#include "libs/primitive/sphere/Sphere.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::Sphere;

TEST(PRIMITIVE, sphere_constructor_stores_radius) {
    Sphere sphere(Vector(0, 0, 0), 2.5);

    ASSERT_DOUBLE_EQ(sphere.radius(), 2.5);
}

TEST(PRIMITIVE, sphere_hits_from_outside) {
    Sphere sphere(Vector(0, 0, 0), 1.0);
    Ray ray(Vector(0, 0, -5), Vector(0, 0, 1));

    const double t = sphere.hits(ray);

    ASSERT_DOUBLE_EQ(t, 4.0);
}

TEST(PRIMITIVE, sphere_miss_returns_negative_one) {
    Sphere sphere(Vector(0, 0, 0), 1.0);
    Ray ray(Vector(0, 0, -5), Vector(0, 1, 0));

    const double t = sphere.hits(ray);

    ASSERT_DOUBLE_EQ(t, -1.0);
}

TEST(PRIMITIVE, sphere_hits_from_inside_uses_second_root) {
    Sphere sphere(Vector(0, 0, 0), 1.0);
    Ray ray(Vector(0, 0, 0), Vector(1, 0, 0));

    const double t = sphere.hits(ray);

    ASSERT_DOUBLE_EQ(t, 1.0);
}

TEST(PRIMITIVE, sphere_surface_data_uv_and_normal) {
    Sphere sphere(Vector(0, 0, 0), 1.0);

    const auto data =
        sphere.surfaceData(raytracer::object::primitive::HitRecord{
            0, -1, 0, raytracer::maths::Vector(0.2, 0.2, 0)});

    ASSERT_DOUBLE_EQ(data.normal.x, 1.0);
    ASSERT_DOUBLE_EQ(data.normal.y, 0.0);
    ASSERT_DOUBLE_EQ(data.normal.z, 0.0);
    ASSERT_DOUBLE_EQ(data.uv.x, 0.5);
    ASSERT_DOUBLE_EQ(data.uv.y, 0.5);
}

TEST(PRIMITIVE, sphere_bounding_box_is_centered_on_sphere) {
    Sphere sphere(Vector(1, 2, 3), 2.0);

    const auto box = sphere.boundingBox();

    ASSERT_DOUBLE_EQ(box.x, -1.0);
    ASSERT_DOUBLE_EQ(box.y, 0.0);
    ASSERT_DOUBLE_EQ(box.z, 1.0);
    ASSERT_DOUBLE_EQ(box.w, 4.0);
    ASSERT_DOUBLE_EQ(box.h, 4.0);
    ASSERT_DOUBLE_EQ(box.d, 4.0);
}