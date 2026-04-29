/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ObjectMiddleware
*/

#pragma once

#include <any>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "exception/PluginException.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"

namespace raytracer::util {
    template <typename T>
    T mapTo(const std::map<std::string, std::any> &mapData,
            std::string_view className);

    class ObjectMiddleware {
      public:
        ObjectMiddleware() = default;
        ~ObjectMiddleware() = default;

        ObjectMiddleware(const ObjectMiddleware &) = default;
        ObjectMiddleware(ObjectMiddleware &&) = default;
        ObjectMiddleware &operator=(const ObjectMiddleware &) = default;
        ObjectMiddleware &operator=(ObjectMiddleware &&) = default;

        /**
         @brief Validates and retrieves an argument from a map of std::any
         objects. This function checks if the specified key exists in the map
         and attempts to cast the argument at that key to the specified type T.
         For Vector and Color types, if the value is a nested map, it will
         automatically convert it using mapTo().
         * @tparam T The expected type of the argument at the specified key.
         * @param params The map of std::any objects containing the arguments
         to validate.
         * @param key The key of the argument to validate and retrieve from
         the map.
         * @param className The name of the class for which the argument is
         being validated, used in error messages.
         * @return An instance of type T if validation and conversion are
         successful.
         * @throws PluginException If the key is not found or if the type
         casting fails, with a message indicating the nature of the error.
         */
        template <typename T>
        static T validate(const std::map<std::string, std::any> &params,
                          const std::string_view key,
                          const std::string_view className) {
            const auto it = params.find(std::string(key));
            if (it == params.end()) {
                throw exception::PluginException{"{} requires parameter '{}'",
                                                 std::string(className),
                                                 std::string(key)};
            }
            if constexpr (std::is_same_v<T, maths::Vector> ||
                          std::is_same_v<T, maths::Color>) {
                try {
                    const auto &nestedMap =
                        std::any_cast<const std::map<std::string, std::any> &>(
                            it->second);
                    return raytracer::util::mapTo<T>(nestedMap, className);
                } catch (const std::bad_any_cast &) {
                }
            }
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException{
                    "{} parameter '{}' has invalid type",
                    std::string{className}, std::string(key)};
            }
        }

        /**
         @brief Check if a double given value it negative or not.
         * @param value double to check if it is negative or not.
         * @param fieldName name of the double to check. For Example radius for
         a Sphere.
         * @param className name of the class of the value.v
         * @throws PluginException If the index is out of bounds or if the type
         casting fails, with a message indicating the nature of the error.
         */
        static void unsignedDouble(double value, std::string_view fieldName,
                                   std::string_view className);

        /**
        @brief Check if a color is valid.
         * @param color color to check.
         * @param className name of the class of the value.v
         * @throws PluginException If the index is out of bounds or if the type
         casting fails, with a message indicating the nature of the error.
         */
        static void color(const maths::Color &color,
                          std::string_view className);
    };
}  // namespace raytracer::util
