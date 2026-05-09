/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_perlinnoise
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

static std::map<std::string, std::any> makePerlinArgs() {
    return std::map<std::string, std::any>{
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)0},
                                                   {"g", (unsigned char)0},
                                                   {"b", (unsigned char)0}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)255},
                                                   {"g", (unsigned char)255},
                                                   {"b", (unsigned char)255}}},
        {"scale", 0.1},
        {"octaves", 3},
        {"persistence", 0.5},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};
}

TEST(PerlinNoiseMaterial, Builds) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto perlinArgs = makePerlinArgs();
    auto matObj = objFactory.build("perlin_noise", perlinArgs);
    ASSERT_NE(matObj, nullptr);
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);
}

TEST(PerlinNoiseMaterial, IntegratesWithPrimitive) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto perlinArgs = makePerlinArgs();
    auto matObj = objFactory.build("perlin_noise", perlinArgs);
    ASSERT_NE(matObj, nullptr);
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"radius", 10.0}};
    auto sphere = objFactory.build("sphere", sphereArgs);
    ASSERT_NE(sphere, nullptr);
    auto basePrim =
        std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(
            sphere);
    ASSERT_NE(basePrim, nullptr);

    // Use a ray that hits the sphere at (0,10,0)
    raytracer::maths::Ray ray(raytracer::maths::Vector(0, 20, 0),
                              raytracer::maths::Vector(0, -1, 0).normalized());
    double dist = basePrim->hits(ray);
    ASSERT_GT(dist, 0);
    raytracer::maths::Vector hitPoint = ray.origin + ray.direction * dist;
    auto sd = basePrim->surfaceData(hitPoint);
    EXPECT_GE(sd.material.color.r, 0);
    EXPECT_LE(sd.material.color.r, 255);
    EXPECT_GE(sd.material.color.g, 0);
    EXPECT_LE(sd.material.color.g, 255);
    EXPECT_GE(sd.material.color.b, 0);
    EXPECT_LE(sd.material.color.b, 255);
}

TEST(PerlinNoiseMaterial, VariesAcrossPoints) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto perlinArgs = makePerlinArgs();
    auto matObj = objFactory.build("perlin_noise", perlinArgs);
    ASSERT_NE(matObj, nullptr);
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"radius", 10.0}};
    auto sphere = objFactory.build("sphere", sphereArgs);
    ASSERT_NE(sphere, nullptr);
    auto basePrim =
        std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(
            sphere);
    ASSERT_NE(basePrim, nullptr);

    // Build samples using explicit constructors to avoid initializer issues
    std::vector<raytracer::maths::Vector> samples;
    samples.push_back(raytracer::maths::Vector(0, 10, 0));
    samples.push_back(raytracer::maths::Vector(1, 10, 0));
    samples.push_back(raytracer::maths::Vector(2, 10, 0));
    samples.push_back(raytracer::maths::Vector(3, 10, 0));
    samples.push_back(raytracer::maths::Vector(4, 10, 0));

    std::set<int> uniqueColors;
    for (const auto &pt : samples) {
        // shoot a ray straight down from y+10 to hit at pt
        raytracer::maths::Ray r(
            raytracer::maths::Vector(pt.x, pt.y + 10.0, pt.z),
            raytracer::maths::Vector(0, -1, 0).normalized());
        double dist = basePrim->hits(r);
        ASSERT_GT(dist, 0);
        raytracer::maths::Vector hitPt = r.origin + r.direction * dist;
        auto sd = basePrim->surfaceData(hitPt);
        int packed = (static_cast<int>(sd.material.color.r) << 16) |
                     (static_cast<int>(sd.material.color.g) << 8) |
                     (static_cast<int>(sd.material.color.b));
        uniqueColors.insert(packed);
    }

    EXPECT_GT(uniqueColors.size(), 1u);
}
