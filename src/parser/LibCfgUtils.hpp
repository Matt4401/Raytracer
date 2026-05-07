/*
** EPITECH PROJECT, 2026
** LibCfgUtils
** File description:
** LibCfgUtils header
*/

#pragma once

#include <any>
#include <libconfig.h++>
#include <map>
#include <string>
#include <string_view>
#include <vector>
namespace raytracer::parsing {

    class LibCfgUtils {
      public:
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
        static std::map<std::string, std::any> groupToMap(
            const libconfig::Setting &group);

        /**
         * @brief Parses a libconfig array setting into a vector.
         * @param array The setting array.
         * @return A vector containing the extracted elements.
         */
        static std::vector<std::any> arrayToVector(
            const libconfig::Setting &array);

        static std::vector<std::string> stringAggregateToVector(
            libconfig::Setting &root, const std::string_view &aggregateName);
    };
}  // namespace raytracer::parsing
