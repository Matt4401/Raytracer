/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser header
*/

#pragma once

#include <any>
#include <filesystem>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>

#include "object/IObject.hpp"

namespace raytracer::parsing {

    class ConfigParser {
        /**
         * @brief Callback function type used to create objects from
         * configuration parameters.
         * @param name The name type of the object to create.
         * @param param A map of parameters for object construction.
         * @return A unique pointer to the newly constructed object.
         */
        using buildCallback = std::function<std::unique_ptr<object::IObject>(
            const std::string &name,
            const std::map<std::string, std::any> &param)>;

        /**
         * @brief Callback function type used to assign constructed objects to
         * the scene.
         * @param object The constructed object ready to be added.
         */
        using AssignCallback =
            std::function<void(std::unique_ptr<raytracer::object::IObject>)>;

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
         * @brief Sets the callback used to assign objects successfully built.
         * @param callback The function to call when an object has been built.
         */
        void setAssignCallback(const AssignCallback &callback);

        /**
         * @brief Parses a configuration file.
         * @param filepath The path to the configuration file.
         */
        void parse(const std::filesystem::path &filepath);

      private:
        buildCallback _buildCallback = nullptr;
        AssignCallback _assignCallback = nullptr;

        libconfig::Config _cfg;

        /**
         * @brief Opens and reads the libconfig file.
         * @param filepath The file to open.
         */
        void loadConfig(const std::filesystem::path &filepath);

        /**
         * @brief Extracts a single value from a setting and converts it to
         * std::any.
         * @param setting The libconfig setting to extract.
         * @return The extracted value as std::any.
         */
        static std::any extractValue(const libconfig::Setting &setting);

        /**
         * @brief Resolves a setting to its appropriate type, acting recursively
         * if needed.
         * @param value The libconfig setting to resolve.
         * @return The resolved value.
         */
        static std::any resolveValue(const libconfig::Setting &value);

        /**
         * @brief Parses a libconfig group setting into a map.
         * @param group The setting group.
         * @return A map linking names to their corresponding extracted values.
         */
        static std::map<std::string, std::any> parseGroup(
            const libconfig::Setting &group);

        /**
         * @brief Parses a libconfig array setting into a vector.
         * @param array The setting array.
         * @return A vector containing the extracted elements.
         */
        static std::vector<std::any> parseArray(
            const libconfig::Setting &array);

        /**
         * @brief Triggers the build callback and, if successful, the assign
         * callback.
         * @param stringName Name element indicating the targeted name.
         * @param config The map of parameters.
         */
        void buildAndAssign(const std::string &stringName,
                            const std::map<std::string, std::any> &config);

        /**
         * @brief Computes a list of objects under a single group definition.
         *        Ex: a list of spheres under a "spheres" group.
         * @param stringName Name string representing the collection.
         * @param group The setting group containing the objects.
         */
        void parseObjectList(const std::string &stringName,
                             const libconfig::Setting &list);

        void computeUniqueObjects(const std::map<std::string, std::any> &data,
                                  const std::string &typeName);

        /**
         * @brief Parses the list of general groupings from the config.
         *        Ex: primitive groups, light groups, etc.
         * @param list The libconfig root list/group list.
         */
        void parseGroups(const libconfig::Setting &list);
    };

}  // namespace raytracer::parsing
