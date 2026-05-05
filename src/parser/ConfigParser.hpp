/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser header
*/

#pragma once

#include <any>
#include <filesystem>
#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "object/IScene.hpp"

namespace raytracer::parsing {

    class ConfigParser {
        /**
         * @brief Callback function type used to create objects from
         * configuration parameters.
         * @param name The name type of the object to create.
         * @param param A map of parameters for object construction.
         * @return A unique pointer to the newly constructed object.
         */
        using buildCallback = std::function<std::shared_ptr<object::IObject>(
            const std::string &name,
            const std::map<std::string, std::any> &param)>;

      public:
        ConfigParser() = default;
        ~ConfigParser() = default;

        /**
         * @brief Sets the callback used to build objects.
         * @param callback The function to call when an object needs to be
         * built.
         */
        void setBuildCallback(const buildCallback &callback);

        /**
         * @brief Parses a configuration file.
         * @param filepath The path to the configuration file.
         * @return list of scene parsed in the file. The first of the list will
         * be the filepath scene. The others will be the others defined in it
         */
        std::vector<std::shared_ptr<object::scene::IScene>> parse(
            const std::filesystem::path &filepath);

      private:
        struct ObjectInfo {
            std::map<std::string, std::any> params;
            std::string name = "";
        };

        buildCallback _buildCallback = nullptr;
        libconfig::Config _cfg;
        std::vector<std::shared_ptr<object::scene::IScene>> _scenes = {};

        static constexpr std::string_view K_SCENE_PARAMETERS =
            "sceneParameters";
        static constexpr std::string_view K_PARAMETERS_KEYWORD = "parameters";
        static constexpr std::string_view K_NAME_KEYWORD = "name";
        static constexpr std::string_view K_MATERIAL_KEYWORD = "material";

        ObjectInfo getObjectInfo(const libconfig::Setting &objectName);

        /**
         * @brief Opens and reads the libconfig file.
         * @param filepath The file to open.
         */
        void loadConfig(const std::filesystem::path &filepath);

        /**
         * @brief Triggers the build callback and, if successful, the assign
         * callback.
         * @param stringName Name element indicating the targeted name.
         * @param config The map of parameters.
         */
        void buildAndAssign(
            const ObjectInfo &info,
            const std::function<void(std::shared_ptr<object::IObject> &)>
                &func);

        /**
         * @brief Computes a list of objects under a single group definition.
         *        Ex: a list of spheres under a "spheres" group.
         * @param stringName Name string representing the collection.
         * @param group The setting group containing the objects.
         */
        void parseObjectList(const libconfig::Setting &list);

        void computeObject(const std::string &parentName,
                           const libconfig::Setting &objectData);

        /**
         * @brief Parses the list of general groupings from the config.
         *        Ex: primitive groups, light groups, etc.
         * @param list The libconfig root list/group list.
         */
        void parseGroups(const libconfig::Setting &list);

        std::shared_ptr<object::scene::IScene> makeScene(
            libconfig::Setting &root);
    };

}  // namespace raytracer::parsing
