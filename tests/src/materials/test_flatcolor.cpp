/*
** EPITECH PROJECT, 2026
** test_flatcolor
** File description:
** test_flatcolor code
*/

#include <gtest/gtest.h>
#include <memory>
#include <any>
#include <map>
#include <string>

#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"
#include "math/Vector.hpp"
#include "math/Color.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/material/IMaterial.hpp"

#ifndef TEST_PLUGINS_PATH
#define TEST_PLUGINS_PATH "./plugins/"
#endif

TEST(MATERIAL, flatcolor_decorator) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    std::map<std::string, std::any> flatColorArgs = {
        {"color", raytracer::maths::Color(255, 0, 0)},
        {"emission", raytracer::maths::Vector(0, 0, 0)},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}
    };
    auto matObj = objFactory.build("flatcolor", flatColorArgs);
    ASSERT_NE(matObj, nullptr);
    auto matPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center", raytracer::maths::Vector(0, 0, 0)},
        {"radius", 10.0}
    };
    auto sphere = objFactory.build("sphere", sphereArgs);
    ASSERT_NE(sphere, nullptr);

    auto basePrim = std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(sphere);
    ASSERT_NE(basePrim, nullptr);
    auto finalData = basePrim->surfaceData(raytracer::maths::Vector(0, 10, 0));

    ASSERT_DOUBLE_EQ(finalData.normal.y, 1.0);

    ASSERT_EQ(finalData.color.r, 255);
    ASSERT_EQ(finalData.color.g, 0);
    ASSERT_EQ(finalData.color.b, 0);
}

TEST(MATERIAL, flatcolor_preserves_normal) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    // Create a material and then a sphere using that material
    std::map<std::string, std::any> flatColorArgs = {
        {"color", raytracer::maths::Color(100, 150, 200)},
        {"emission", raytracer::maths::Vector(0, 0, 0)},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}
    };
    auto matObj = objFactory.build("flatcolor", flatColorArgs);
    ASSERT_NE(matObj, nullptr);
    auto matPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center", raytracer::maths::Vector(5, 5, 5)},
        {"radius", 2.0}
    };
    auto sphere = objFactory.build("sphere", sphereArgs);
    ASSERT_NE(sphere, nullptr);
    auto basePrim = std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(sphere);

    raytracer::maths::Vector hitPoint(7, 5, 5);
    auto data = basePrim->surfaceData(hitPoint);

    ASSERT_DOUBLE_EQ(data.normal.x, 1.0);
    ASSERT_NEAR(data.normal.y, 0.0, 1e-10);
    ASSERT_NEAR(data.normal.z, 0.0, 1e-10);

    ASSERT_EQ(data.color.r, 100);
    ASSERT_EQ(data.color.g, 150);
    ASSERT_EQ(data.color.b, 200);
}
