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

        void parse(const std::shared_ptr<object::scene::IScene> &sceneBuffer,
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
        static constexpr std::string_view K_TRANSFORMATIONS_KEYWORD =
            "transformations";
        static constexpr std::string_view K_SCENES_KEYWORD = "scenes";
        static constexpr std::string_view K_EXTRA_KEYWORD = "objects_clusters";

        /**
         * @brief Extracts object information from a libconfig setting.
         * @param objectName The libconfig setting containing the object data.
         * @return An ObjectInfo struct containing the object's name and
         * parameters.
         */
        ObjectInfo getObjectInfo(const libconfig::Setting &objectName);

        /**
         * @brief Opens and reads the libconfig file.
         * @param filepath The file to open.
         */
        void loadConfig(const std::filesystem::path &filepath);

        /**
         * @brief Triggers the build callback and, if successful, the assign
         * callback.
         * @param info The ObjectInfo containing name and parameters.
         * @param func The callback function to execute on the built object.
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

        /**
         * @brief Processes a single object from the configuration.
         * @param parentName The name of the parent group (e.g., "sphere",
         * "plane").
         * @param objectData The libconfig setting containing the object's data.
         */
        void computeObject(const std::string &parentName,
                           const libconfig::Setting &objectData);

        /**
         * @brief Parses the list of general groupings from the config.
         *        Ex: primitive groups, light groups, etc.
         * @param list The libconfig root list/group list.
         */
        void parseGroups(const libconfig::Setting &list);

        /**
         * @brief Processes and attaches a material definition to an object.
         * @param info The ObjectInfo to attach the material to.
         * @param objectData The libconfig setting containing the material data.
         */
        void computeMaterial(ObjectInfo &info,
                             const libconfig::Setting &objectData);

        std::shared_ptr<object::IObject> computeTransforms(
            const std::shared_ptr<object::IObject> &baseObject,
            const libconfig::Setting &objectData);

        void parseObjects(libconfig::Setting &root,
                          const std::filesystem::path &path);

        void subFileHandling(
            const std::vector<std::string> &scenesPath,
            const std::filesystem::path &currentPath,
            const std::function<void(ConfigParser &parser,
                                     const std::filesystem::path &path)> &func);

        /**
         * @brief Creates a scene from the parsed configuration root.
         * @param root The root libconfig setting containing all scene data.
         */
        void makeScene(libconfig::Setting &root);
    };

}  // namespace raytracer::parsing
