/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_marble
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <set>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

static std::map<std::string, std::any> makeMarbleArgs() {
    return std::map<std::string, std::any>{
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)240},
                                                   {"g", (unsigned char)240},
                                                   {"b", (unsigned char)240}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)40},
                                                   {"g", (unsigned char)40},
                                                   {"b", (unsigned char)45}}},
        {"scale", 0.4},
        {"octaves", 5},
        {"persistence", 0.6},
        {"turbulence", 8.0},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};
}

TEST(MarbleMaterial, Builds) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeMarbleArgs();
    auto matObj = objFactory.build("marble", args);
    ASSERT_NE(matObj, nullptr);
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);
}

TEST(MarbleMaterial, VariesAcrossPoints) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    auto args = makeMarbleArgs();
    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            objFactory.build("marble", args));

    std::map<std::string, std::any> sphereArgs = {
        {"material", matPtr},
        {"center",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"radius", 10.0}};
    auto sphere =
        std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(
            objFactory.build("sphere", sphereArgs));

    std::set<int> uniqueColors;
    for (int i = 0; i < 10; ++i) {
        auto sd = sphere->surfaceData(
            raytracer::object::primitive::HitRecord{0, -1, 0,
                                                    raytracer::maths::Vector(i * 0.5, 10.0, 0)});
        int packed = (sd.material.color.r << 16) | (sd.material.color.g << 8) |
                     sd.material.color.b;
        uniqueColors.insert(packed);
    }

    EXPECT_GT(uniqueColors.size(), 1u);
}
