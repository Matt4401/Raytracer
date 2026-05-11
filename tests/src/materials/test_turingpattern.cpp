/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_turingpattern
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <string>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

static std::map<std::string, std::any> makeTuringArgs() {
    return std::map<std::string, std::any>{
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)50},
                                                    {"g", (unsigned char)50},
                                                    {"b", (unsigned char)50}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)200},
                                                    {"g", (unsigned char)200},
                                                    {"b", (unsigned char)200}}},
        {"scale", 0.05},
        {"octaves", 4},
        {"persistence", 0.6},
        {"offset", 0.0},
        {"threshold", 0.2},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};
}

TEST(TuringPatternMaterial, Builds) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeTuringArgs();
    auto matObj = objFactory.build("turing_pattern", args);
    ASSERT_NE(matObj, nullptr);
    auto matPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(matObj);
    ASSERT_NE(matPtr, nullptr);
}

TEST(TuringPatternMaterial, IntegratesWithPrimitive) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeTuringArgs();
    auto matObj = objFactory.build("turing_pattern", args);
    ASSERT_NE(matObj, nullptr);
    auto matPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center", std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"radius", 10.0}};
    auto sphere = objFactory.build("sphere", sphereArgs);
    ASSERT_NE(sphere, nullptr);
    auto basePrim = std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(sphere);
    ASSERT_NE(basePrim, nullptr);

    auto sd = basePrim->surfaceData(raytracer::maths::Vector(0, 10, 0));
    EXPECT_GE(sd.material.color.r, 0);
    EXPECT_LE(sd.material.color.r, 255);
    EXPECT_GE(sd.material.color.g, 0);
    EXPECT_LE(sd.material.color.g, 255);
    EXPECT_GE(sd.material.color.b, 0);
    EXPECT_LE(sd.material.color.b, 255);
}
