/*
** EPITECH PROJECT, 2026
** test_cone
** File description:
** Unit tests for Cone primitive
*/

#include <gtest/gtest.h>

#include "exception/PluginException.hpp"
#include "libs/primitive/cone/Cone.hpp"
#include "libs/primitive/cone/LimitedCone.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"

using raytracer::maths::Ray;
using raytracer::maths::Vector;
using raytracer::object::primitive::Cone;
using raytracer::object::primitive::LimitedCone;

TEST(PRIMITIVE, cone_hits_from_outside) {
    Cone cone(Vector(0, 0, 0), 1.0);
    Ray ray(Vector(0, 0, -5), Vector(0, 0, 1));

    const double t = cone.hits(ray);

    ASSERT_DOUBLE_EQ(t, 5.0);
}

TEST(PRIMITIVE, cone_negative_k_throws) {
    EXPECT_THROW((void)Cone(Vector(0, 0, 0), -1.0),
                 raytracer::exception::PluginException);
}

TEST(PRIMITIVE, limited_cone_rejects_side_out_of_height) {
    LimitedCone lc(Vector(0, 0, 0), 1.0, 2.0, false);
    Ray ray(Vector(0, 2.5, -5), Vector(0, 0, 1));

    const double t = lc.hits(ray);

    ASSERT_DOUBLE_EQ(t, -1.0);
}

TEST(PRIMITIVE, limited_cone_closed_prefers_cap_over_side) {
    LimitedCone openCone(Vector(0, 0, 0), 1.0, 2.0, false);

    std::map<std::string, std::any> args;
    args["center"] = std::map<std::string, std::any>{{"x", 0.0},
                                                      {"y", 0.0},
                                                      {"z", 0.0}};
    args["k"] = 1.0;
    args["height"] = 2.0;
    args["closed"] = true;
    args["material"] = std::shared_ptr<raytracer::object::material::IMaterial>{};
    LimitedCone closedCone(args);

    Ray ray(Vector(0.5, 2.0, 0.0), Vector(0, -1, 0));

    const double tOpen = openCone.hits(ray);
    const double tClosed = closedCone.hits(ray);

    ASSERT_DOUBLE_EQ(tOpen, 1.5);
    ASSERT_DOUBLE_EQ(tClosed, 1.0);
}

TEST(PRIMITIVE, limited_cone_negative_height_throws) {
    EXPECT_THROW((void)LimitedCone(Vector(0, 0, 0), 1.0, -2.0, true),
                 raytracer::exception::PluginException);
}
