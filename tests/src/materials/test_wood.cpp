/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_wood
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

static std::map<std::string, std::any> makeWoodArgs() {
    return std::map<std::string, std::any>{
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)160},
                                                   {"g", (unsigned char)105},
                                                   {"b", (unsigned char)60}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)80},
                                                   {"g", (unsigned char)45},
                                                   {"b", (unsigned char)20}}},
        {"scale", 0.8},
        {"octaves", 3},
        {"persistence", 0.5},
        {"turbulence", 3.0},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};
}

TEST(WoodMaterial, Builds) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeWoodArgs();
    auto matObj = objFactory.build("wood", args);
    ASSERT_NE(matObj, nullptr);
}

TEST(WoodMaterial, SolidTexturingConsistency) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeWoodArgs();
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            objFactory.build("wood", args));

    auto sd1 = matPtr->evaluate({}, raytracer::maths::Vector(10, 0, 0));
    auto sd2 = matPtr->evaluate({}, raytracer::maths::Vector(0, 0, 10));

    EXPECT_GE(sd1.color.r, 0);
    EXPECT_LE(sd1.color.r, 255);
}