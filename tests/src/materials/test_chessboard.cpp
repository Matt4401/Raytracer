/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** test_chessboard
*/

#include <gtest/gtest.h>

#include <any>
#include <map>
#include <memory>
#include <string>

#include "../libs/materials/chessboard/Chessboard.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

#ifndef TEST_PLUGINS_PATH
#define TEST_PLUGINS_PATH "./plugins/"
#endif

TEST(ChessboardMaterial, BasicTiles) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
#ifdef PROJECT_PLUGINS_PATH
    plugManager.updatePluginList(PROJECT_PLUGINS_PATH);
#endif
    plugManager.fillFactory(objFactory);

    std::map<std::string, std::any> chessArgs = {
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)255},
                                                   {"g", (unsigned char)255},
                                                   {"b", (unsigned char)255}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)0},
                                                   {"g", (unsigned char)0},
                                                   {"b", (unsigned char)0}}},
        {"tileSize", 1.0},
        {"emission",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};

    auto matObj = objFactory.build("chessboard", chessArgs);
    ASSERT_NE(matObj, nullptr);

    auto matPtr =
        std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
            matObj);
    ASSERT_NE(matPtr, nullptr);

    std::map<std::string, std::any> planeArgs = {
        {"material", matPtr},
        {"center",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}},
        {"normal",
         std::map<std::string, std::any>{{"x", 0.0}, {"y", 1.0}, {"z", 0.0}}}};

    auto planeObj = objFactory.build("plane", planeArgs);
    ASSERT_NE(planeObj, nullptr);

    auto basePrim =
        std::dynamic_pointer_cast<raytracer::object::primitive::IPrimitive>(
            planeObj);
    ASSERT_NE(basePrim, nullptr);

    // Ray to hit at (0.2, 0.0, 0.0)
    raytracer::maths::Ray ray1(
        raytracer::maths::Vector(0.2, 10.0, 0.0),
        raytracer::maths::Vector(0.0, -1.0, 0.0).normalized());
    double dist1 = basePrim->hits(ray1);
    ASSERT_GT(dist1, 0);
    raytracer::maths::Vector hitPoint1 = ray1.origin + ray1.direction * dist1;
    auto sd1 = basePrim->surfaceData(hitPoint1);
    EXPECT_EQ(sd1.material.color.r, 0);
    EXPECT_EQ(sd1.material.color.g, 0);
    EXPECT_EQ(sd1.material.color.b, 0);

    // Ray to hit at (1.2, 0.0, 0.0)
    raytracer::maths::Ray ray2(
        raytracer::maths::Vector(1.2, 10.0, 0.0),
        raytracer::maths::Vector(0.0, -1.0, 0.0).normalized());
    double dist2 = basePrim->hits(ray2);
    ASSERT_GT(dist2, 0);
    raytracer::maths::Vector hitPoint2 = ray2.origin + ray2.direction * dist2;
    auto sd2 = basePrim->surfaceData(hitPoint2);
    EXPECT_EQ(sd2.material.color.r, 255);
    EXPECT_EQ(sd2.material.color.g, 255);
    EXPECT_EQ(sd2.material.color.b, 255);
}

TEST(ChessboardMaterial, InvalidTileSizeThrows) {
    std::map<std::string, std::any> badArgs = {
        {"color1", std::map<std::string, std::any>{{"r", (unsigned char)0},
                                                   {"g", (unsigned char)0},
                                                   {"b", (unsigned char)0}}},
        {"color2", std::map<std::string, std::any>{{"r", (unsigned char)255},
                                                   {"g", (unsigned char)255},
                                                   {"b", (unsigned char)255}}},
        {"tileSize", 0.0},
        {"reflType", raytracer::object::primitive::RefltT::DIFF}};

    EXPECT_THROW(
        {
            raytracer::object::material::Chessboard cb(badArgs);
            (void)cb;
        },
        raytracer::exception::PluginException);
}