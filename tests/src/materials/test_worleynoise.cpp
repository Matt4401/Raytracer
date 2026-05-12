/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_worleynoise
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

static std::map<std::string, std::any> makeWorleyArgs() {
    return std::map<std::string, std::any>{
        {"color1", std::map<std::string, std::any>{{"r", 0},
                                                   {"g", 0},
                                                   {"b", 0}}},
        {"color2", std::map<std::string, std::any>{{"r", 255},
                                                   {"g", 255},
                                                   {"b", 255}}},
        {"scale", 0.5},
        {"reflType", std::string("DIFF")}};
}

TEST(WorleyNoiseMaterial, Builds) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeWorleyArgs();
    auto matObj = objFactory.build("worley_noise", args);
    ASSERT_NE(matObj, nullptr);
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);
}

TEST(WorleyNoiseMaterial, IntegratesWithPrimitive) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeWorleyArgs();
    auto matObj = objFactory.build("worley_noise", args);
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

    auto sd = basePrim->surfaceData(raytracer::maths::Vector(0, 10, 0));
    EXPECT_GE(sd.material.color.r, 0);
    EXPECT_LE(sd.material.color.r, 255);
    EXPECT_GE(sd.material.color.g, 0);
    EXPECT_LE(sd.material.color.g, 255);
    EXPECT_GE(sd.material.color.b, 0);
    EXPECT_LE(sd.material.color.b, 255);
}

TEST(WorleyNoiseMaterial, VariesAcrossPoints) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeWorleyArgs();
    auto matObj = objFactory.build("worley_noise", args);
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

    std::vector<raytracer::maths::Vector> samples;
    samples.push_back(raytracer::maths::Vector(0, 10, 0));
    samples.push_back(raytracer::maths::Vector(1, 10, 0));
    samples.push_back(raytracer::maths::Vector(2, 10, 0));
    samples.push_back(raytracer::maths::Vector(3, 10, 0));
    samples.push_back(raytracer::maths::Vector(4, 10, 0));

    std::set<int> uniqueColors;
    for (const auto &pt : samples) {
        auto sd = basePrim->surfaceData(pt);
        int packed = (static_cast<int>(sd.material.color.r) << 16) |
                     (static_cast<int>(sd.material.color.g) << 8) |
                     (static_cast<int>(sd.material.color.b));
        uniqueColors.insert(packed);
    }

    EXPECT_GT(uniqueColors.size(), 1u);
}
