/*
** EPITECH PROJECT, 2026
** test_triangle
** File description:
** test_triangle code
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <string>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

#ifndef TEST_PLUGINS_PATH
#define TEST_PLUGINS_PATH "./plugins/"
#endif

namespace {
    std::shared_ptr<raytracer::object::material::IMaterial> buildFlatColor(
        raytracer::ObjectFactory &factory) {
        std::map<std::string, std::any> flatColorArgs = {
            {"color",
             std::map<std::string, std::any>{{"r", (unsigned char)200},
                                             {"g", (unsigned char)200},
                                             {"b", (unsigned char)200}}},
            {"emission",
             std::map<std::string, std::any>{
                 {"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
            {"reflType", raytracer::object::primitive::RefltT::DIFF}};
        auto matObj = factory.build("flatcolor", flatColorArgs);
        EXPECT_NE(matObj, nullptr);
        return std::dynamic_pointer_cast<
            raytracer::object::material::IMaterial>(matObj);
    }

    std::shared_ptr<raytracer::object::primitive::IPrimitive> buildTriangle(
        raytracer::ObjectFactory &factory,
        const raytracer::maths::Vector &center,
        const raytracer::maths::Vector &v1,
        const raytracer::maths::Vector &v2) {
        auto material = buildFlatColor(factory);
        EXPECT_NE(material, nullptr);
        std::map<std::string, std::any> args = {
            {"material", material},
            {"center",
             std::map<std::string, std::any>{
                 {"x", center.x}, {"y", center.y}, {"z", center.z}}},
            {"v1",
             std::map<std::string, std::any>{
                 {"x", v1.x}, {"y", v1.y}, {"z", v1.z}}},
            {"v2",
             std::map<std::string, std::any>{
                 {"x", v2.x}, {"y", v2.y}, {"z", v2.z}}},
        };

        auto obj = factory.build("triangle", args);
        EXPECT_NE(obj, nullptr);
        return std::dynamic_pointer_cast<
            raytracer::object::primitive::IPrimitive>(obj);
    }

    void initFactory(raytracer::PluginManager &plugManager,
                     raytracer::ObjectFactory &objFactory) {
        plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
        plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
        plugManager.fillFactory(objFactory);
    }
}  // namespace

TEST(PRIMITIVE_TRIANGLE, hits_center) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;
    initFactory(plugManager, objFactory);
    auto tri = buildTriangle(objFactory, raytracer::maths::Vector(0, 0, 0),
                             raytracer::maths::Vector(1, 0, 0),
                             raytracer::maths::Vector(0, 1, 0));
    ASSERT_NE(tri, nullptr);

    raytracer::maths::Ray ray(raytracer::maths::Vector(0.25, 0.25, -1),
                              raytracer::maths::Vector(0, 0, 1));

    const double t = tri->hits(ray);
    ASSERT_GT(t, 0.0);
    ASSERT_NEAR(t, 1.0, 1e-6);
}

TEST(PRIMITIVE_TRIANGLE, misses_outside) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;
    initFactory(plugManager, objFactory);
    auto tri = buildTriangle(objFactory, raytracer::maths::Vector(0, 0, 0),
                             raytracer::maths::Vector(1, 0, 0),
                             raytracer::maths::Vector(0, 1, 0));
    ASSERT_NE(tri, nullptr);

    raytracer::maths::Ray ray(raytracer::maths::Vector(1, 1, -1),
                              raytracer::maths::Vector(0, 0, 1));

    const double t = tri->hits(ray);
    ASSERT_LT(t, 0.0);
}

TEST(PRIMITIVE_TRIANGLE, surface_normal_and_uv) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;
    initFactory(plugManager, objFactory);
    auto tri = buildTriangle(objFactory, raytracer::maths::Vector(0, 0, 0),
                             raytracer::maths::Vector(1, 0, 0),
                             raytracer::maths::Vector(0, 1, 0));
    ASSERT_NE(tri, nullptr);

        const auto data = tri->surfaceData(raytracer::object::primitive::HitRecord{0, -1, 0, raytracer::maths::Vector(0.2, 0.2, 0)});

    ASSERT_NEAR(data.normal.x, 0.0, 1e-6);
    ASSERT_NEAR(data.normal.y, 0.0, 1e-6);
    ASSERT_NEAR(data.normal.z, 1.0, 1e-6);

    ASSERT_NEAR(data.uv.x, 0.2, 1e-6);
    ASSERT_NEAR(data.uv.y, 0.2, 1e-6);
}

TEST(PRIMITIVE_TRIANGLE, bounding_box) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;
    initFactory(plugManager, objFactory);
    auto tri = buildTriangle(objFactory, raytracer::maths::Vector(-1, -2, -3),
                             raytracer::maths::Vector(2, 0, -1),
                             raytracer::maths::Vector(0, 3, 4));
    ASSERT_NE(tri, nullptr);

    const auto box = tri->boundingBox();

    ASSERT_DOUBLE_EQ(box.x, -1);
    ASSERT_DOUBLE_EQ(box.y, -2);
    ASSERT_DOUBLE_EQ(box.z, -3);
    ASSERT_DOUBLE_EQ(box.w, 3);
    ASSERT_DOUBLE_EQ(box.h, 5);
    ASSERT_DOUBLE_EQ(box.d, 7);
}
