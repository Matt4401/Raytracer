/*
** EPITECH PROJECT, 2026
** test_plugin
** File description:
** test_plugin code
*/

#include <gtest/gtest.h>

#include "../../libs/valid_plugin/ValidObject.hpp"
#include "object/IObject.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

#ifndef TEST_PLUGINS_PATH
#define TEST_PLUGINS_PATH "./tests/plugins/"
#endif

TEST(CORE_PLUGIN, valid_plugins) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
    plugManager.fillFactory(objFactory);

    std::shared_ptr<raytracer::object::IObject> object =
        objFactory.build("valid_test", {});

    ASSERT_FALSE(object == nullptr);

    try {
        if (const auto *data =
                dynamic_cast<raytracer::tests::ValidObject *>(object.get())) {
            ASSERT_EQ(raytracer::object::IObject::Type::PRIMITIVE,
                      data->type());
            return;
        }
    } catch (...) {
    }
    FAIL();
}

TEST(CORE_PLUGIN, error_missing_entry) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
    plugManager.fillFactory(objFactory);

    std::shared_ptr<raytracer::object::IObject> object =
        objFactory.build("missing_entry", {});

    ASSERT_TRUE(object == nullptr);
}

TEST(CORE_PLUGIN, mixed_valid_and_invalid_plugins) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
    plugManager.fillFactory(objFactory);

    std::shared_ptr<raytracer::object::IObject> validObject =
        objFactory.build("valid_test", {});
    std::shared_ptr<raytracer::object::IObject> invalidObject =
        objFactory.build("missing_entry", {});

    ASSERT_NE(validObject, nullptr);
    ASSERT_EQ(invalidObject, nullptr);
}

TEST(CORE_PLUGIN, update_plugin_list_idempotent_reload) {
    raytracer::PluginManager plugManager;
    raytracer::ObjectFactory objFactory;

    plugManager.updatePluginList(TEST_PLUGINS_PATH);
    plugManager.fillFactory(objFactory);
    plugManager.updatePluginList(TEST_PLUGINS_PATH);
    plugManager.fillFactory(objFactory);

    std::shared_ptr<raytracer::object::IObject> object =
        objFactory.build("valid_test", {});

    ASSERT_NE(object, nullptr);
}
