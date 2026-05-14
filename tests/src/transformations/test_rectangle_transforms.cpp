/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_rectangle_transforms
*/

#include <gtest/gtest.h>
#include <map>
#include <any>
#include "math/Ray.hpp"
#include "libs/primitive/rectangle/Rectangle.hpp"
#include "../libs/transformations/translation/Translation.hpp"
#include "../libs/transformations/scale/Scale.hpp"
#include "../libs/transformations/rotation/Rotation.hpp"

using namespace raytracer::maths;
using namespace raytracer::object::primitive;

TEST(TRANSFORMS, rectangle_rotation_90_deg) {
    auto rect = std::make_shared<Rectangle>(Vector(1, 0, 0), Vector(1, 2, 3), Vector(4, 0, 0));
    std::map<std::string, std::any> args = {
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(rect)},
        {"x", 0.0}, {"y", 0.0}, {"z", 90.0}
    };
    Rotation rotatedRect(args);

    Ray rayX(Vector(0, 1, 1.5), Vector(1, 0, 0));
    HitRecord rec;
    EXPECT_FALSE(rotatedRect.hits(rayX, rec));

    Ray rayY(Vector(-1, 0, 1.5), Vector(0, 1, 0));
    EXPECT_TRUE(rotatedRect.hits(rayY, rec));
}

TEST(TRANSFORMS, complex_chain_order) {
    auto rect = std::make_shared<Rectangle>(Vector(1, 0, 0), Vector(1, 2, 3), Vector(4, 0, 0));

    auto sMat = std::make_shared<Scale>(std::map<std::string, std::any>{
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(rect)},
        {"x", 1.0}, {"y", 1.0}, {"z", 2.0}
    });

    Translation finalObj(std::map<std::string, std::any>{
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(sMat)},
        {"x", 5.0}, {"y", 0.0}, {"z", 0.0}
    });

    Ray ray(Vector(0, 1, 4), Vector(1, 0, 0));
    HitRecord rec;
    ASSERT_TRUE(finalObj.hits(ray, rec));
    ASSERT_DOUBLE_EQ(rec.hitPoint.x, 6.0);
}