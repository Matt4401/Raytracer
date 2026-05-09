/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_texturedmaterial
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

#ifndef TEST_PLUGINS_PATH
#define TEST_PLUGINS_PATH "./plugins/"
#endif

TEST(MATERIAL, texturedmaterial_basic) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    std::map<std::string, std::any> texArgs = {
        {"texturePath", std::string("../../assets/images/texture.jpg")},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};
    auto matObj = objFactory.build("textured_material", texArgs);
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

    raytracer::maths::Ray ray(raytracer::maths::Vector(0, 20, 0),
                              raytracer::maths::Vector(0, -1, 0).normalized());
    auto hitCtx = basePrim->hits(ray);
    ASSERT_TRUE(hitCtx.has_value());
    auto finalData = hitCtx->surfaceData;

    ASSERT_GE(static_cast<int>(finalData.material.color.r), 0);
    ASSERT_LE(static_cast<int>(finalData.material.color.r), 255);
    ASSERT_GE(static_cast<int>(finalData.material.color.g), 0);
    ASSERT_LE(static_cast<int>(finalData.material.color.g), 255);
    ASSERT_GE(static_cast<int>(finalData.material.color.b), 0);
    ASSERT_LE(static_cast<int>(finalData.material.color.b), 255);
}
