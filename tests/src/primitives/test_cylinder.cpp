/*
** EPITECH PROJECT, 2026
** test_cylinder
** File description:
** Unit tests for Cylinder primitive
*/

#include <gtest/gtest.h>

#include "exception/PluginException.hpp"
#include "libs/primitive/cylinder/Cylinder.hpp"
#include "libs/primitive/cylinder/LimitedCylinder.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::Cylinder;
using raytracer::object::primitive::LimitedCylinder;

TEST(PRIMITIVE, cylinder_hits_from_outside) {
    Cylinder cyl(Vector(0, 0, 0), 1.0);
    Ray ray(Vector(0, 0, -5), Vector(0, 0, 1));

    const double t = cyl.hits(ray);

    ASSERT_DOUBLE_EQ(t, 4.0);
}

TEST(PRIMITIVE, cylinder_negative_radius_throws) {
    EXPECT_THROW((void)Cylinder(Vector(0, 0, 0), -1.0),
                 raytracer::exception::PluginException);
}

TEST(PRIMITIVE, limited_cylinder_rejects_side_out_of_height) {
    LimitedCylinder lc(Vector(0, 0, 0), 1.0, 2.0, false);
    Ray ray(Vector(0, 2.5, -5), Vector(0, 0, 1));

    const double t = lc.hits(ray);

    ASSERT_DOUBLE_EQ(t, -1.0);
}

TEST(PRIMITIVE, limited_cylinder_open_misses_cap_hit) {
    LimitedCylinder lc(Vector(0, 0, 0), 1.0, 2.0, false);
    Ray ray(Vector(0, 2, 0), Vector(0, -1, 0));

    const double t = lc.hits(ray);

    ASSERT_DOUBLE_EQ(t, -1.0);
}

TEST(PRIMITIVE, limited_cylinder_closed_hits_cap) {
    std::map<std::string, std::any> args;
    args["center"] =
        std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}};
    args["radius"] = 1.0;
    args["height"] = 2.0;
    args["closed"] = true;
    args["material"] =
        std::shared_ptr<raytracer::object::material::IMaterial>{};

    LimitedCylinder lc(args);
    Ray ray(Vector(0, 2, 0), Vector(0, -1, 0));

    const double t = lc.hits(ray);

    ASSERT_DOUBLE_EQ(t, 1.0);
}

TEST(PRIMITIVE, limited_cylinder_negative_height_throws) {
    EXPECT_THROW((void)LimitedCylinder(Vector(0, 0, 0), 1.0, -2.0, true),
                 raytracer::exception::PluginException);
}
