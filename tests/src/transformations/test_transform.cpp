/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_transforms
*/

#include <gtest/gtest.h>
#include <any>
#include <map>
#include <memory>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "libs/primitive/sphere/Sphere.hpp"
#include "object/IObject.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "../libs/transformations/translation/Translation.hpp"
#include "../libs/transformations/scale/Scale.hpp"
#include "../libs/transformations/rotation/Rotation.hpp"

using namespace raytracer::maths;
using namespace raytracer::object::primitive;

static std::shared_ptr<IPrimitive> createUnitSphere() {
    return std::make_shared<Sphere>(Vector(0,0,0), 1.0);
}

TEST(TRANSFORMS, translation_moves_object_correctly) {
    auto sphere = createUnitSphere();
    std::map<std::string, std::any> transArgs = {
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(sphere)},
        {"x", 0.0}, {"y", 5.0}, {"z", 0.0}
    };
    Translation translatedSphere(transArgs);

    Ray hittingRay(Vector(0, 5, -5), Vector(0, 0, 1));
    HitRecord record;
    EXPECT_TRUE(translatedSphere.hits(hittingRay, record));
    EXPECT_DOUBLE_EQ(record.t, 4.0);

    Ray missingRay(Vector(0, 0, -5), Vector(0, 0, 1));
    EXPECT_FALSE(translatedSphere.hits(missingRay, record));
}

TEST(TRANSFORMS, scale_resizes_object_correctly) {
    auto sphere = createUnitSphere();
    std::map<std::string, std::any> scaleArgs = {
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(sphere)},
        {"x", 2.0}, {"y", 2.0}, {"z", 2.0}
    };
    Scale scaledSphere(scaleArgs);

    Ray ray(Vector(0, 0, -5), Vector(0, 0, 1));
    HitRecord record;
    EXPECT_TRUE(scaledSphere.hits(ray, record));
    EXPECT_DOUBLE_EQ(record.t, 3.0);
}

TEST(TRANSFORMS, chained_transformations_order_matters) {
    auto sphere = createUnitSphere();
    Translation translatedSphere(std::map<std::string, std::any>{
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(sphere)},
        {"x", 5.0}, {"y", 0.0}, {"z", 0.0}
    });

    Rotation finalSphere(std::map<std::string, std::any>{
        {"primitive", std::dynamic_pointer_cast<raytracer::object::IObject>(
            std::make_shared<Translation>(translatedSphere))},
        {"x", 0.0}, {"y", 0.0}, {"z", 90.0}
    });

    Ray hittingRay(Vector(0, 5, -5), Vector(0, 0, 1));
    HitRecord record;
    EXPECT_TRUE(finalSphere.hits(hittingRay, record));

    Ray missingRay(Vector(5, 0, -5), Vector(0, 0, 1));
    EXPECT_FALSE(finalSphere.hits(missingRay, record));
}
