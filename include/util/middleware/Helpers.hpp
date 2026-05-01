/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Helpers
*/

#pragma once

#include <any>
#include <map>
#include <math/Color.hpp>
#include <math/Vector.hpp>
#include <string>
#include <string_view>

namespace raytracer::util {
    class Helpers {
      public:
        Helpers() = default;
        ~Helpers() = default;

        Helpers(const Helpers &) = default;
        Helpers(Helpers &&) = default;
        Helpers &operator=(const Helpers &) = default;
        Helpers &operator=(Helpers &&) = default;

        /**
             @brief Convert a nested map parameter to a Color object.
             * @param params The parameter map containing the color information.
             * @param keyName The key to look up in the parameter map for the
           color information.
             * @param className The name of the class requesting the color
             conversion, used for error messages.
             * @return A Color object constructed from the nested map
           parameters.
             * @throws PluginException If the key is missing, the type is
           invalid, or if the color components are out of the valid range
           (0-255).
             */
        static maths::Color toColor(
            const std::map<std::string, std::any> &params,
            std::string_view keyName, std::string_view className);

        /**
         @brief Convert a nested map parameter to a Vector object.
         * @param params The parameter map containing the vector information.
         * @param keyName The key to look up in the parameter map for the vector
         information.
         * @param className The name of the class requesting the vector
         conversion, used for error messages.
         * @return A Vector object constructed from the nested map parameters.
         * @throws PluginException If the key is missing, the type is invalid,
         or if the vector components cannot be converted to double.
         */
        static maths::Vector toVector(
            const std::map<std::string, std::any> &params,
            std::string_view keyName, std::string_view className);

        /**
         @brief Convert an optional nested map parameter to a Color object.
         * @param params The parameter map containing the color information.
         * @param keyName The key to look up in the parameter map for the color
         information.
         * @param defaultValue Color to return if the key is not found.
         * @param className The name of the class requesting the color
         conversion, used for error messages.
         * @return A Color object if key exists, otherwise the default value.
         * @throws PluginException If the key exists but type/conversion fails.
         */
        static maths::Color optionalColor(
            const std::map<std::string, std::any> &params,
            std::string_view keyName, const maths::Color &defaultValue,
            std::string_view className);

        /**
         @brief Convert an optional nested map parameter to a Vector object.
         * @param params The parameter map containing the vector information.
         * @param keyName The key to look up in the parameter map for the vector
         information.
         * @param defaultValue Vector to return if the key is not found.
         * @param className The name of the class requesting the vector
         conversion, used for error messages.
         * @return A Vector object if key exists, otherwise the default value.
         * @throws PluginException If the key exists but type/conversion fails.
         */
        static maths::Vector optionalVector(
            const std::map<std::string, std::any> &params,
            std::string_view keyName, const maths::Vector &defaultValue,
            std::string_view className);

        /**
             @brief Check if a double given value it negative or not.
             * @param value double to check if it is negative or not.
             * @param fieldName name of the double to check. For Example radius
           for a Sphere.
             * @param className name of the class of the value.v
             * @throws PluginException If the index is out of bounds or if the
           type casting fails, with a message indicating the nature of the
           error.
             */
        static void unsignedDouble(double value, std::string_view fieldName,
                                   std::string_view className);
        /**
             @brief Check if an integer given value is negative or not.
             * @param value integer to check if it is negative or not.
             * @param fieldName name of the integer to check. For Example
           samples for a Sphere.
             * @param className name of the class of the value.
             * @throws PluginException If the index is out of bounds or if the
           type casting fails, with a message indicating the nature of the
           error.
             */
        static void unsignedInt(const int value,
                                const std::string_view fieldName,
                                const std::string_view className);
    };
}  // namespace raytracer::util
