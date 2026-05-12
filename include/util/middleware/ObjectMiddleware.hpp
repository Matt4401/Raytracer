/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ObjectMiddleware
*/

#pragma once

#include <any>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>

#include "exception/PluginException.hpp"

namespace raytracer::util {
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
         objects.
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
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException{
                    "{} parameter '{}' has invalid type",
                    std::string{className}, std::string(key)};
            }
        }

        // Overloads for unordered_map to support SurfaceData::extraParams
        template <typename T>
        static T validate(
            const std::unordered_map<std::string, std::any> &params,
            const std::string_view key, const std::string_view className) {
            const auto it = params.find(std::string(key));
            if (it == params.end()) {
                throw exception::PluginException{"{} requires parameter '{}'",
                                                 std::string(className),
                                                 std::string(key)};
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
         * @brief Retrieve an optional parameter from a parameter map.
         * @tparam T The type of the parameter.
         * @param params The parameter map.
         * @param key The key of the parameter.
         * @param defaultValue The default value if the key is not found.
         * @param className The name of the class requesting the parameter.
         * @return The value of the parameter or the default value.
         */
        template <typename T>
        static T optional(const std::map<std::string, std::any> &params,
                          const std::string_view key, const T &defaultValue,
                          const std::string_view className) {
            const auto it = params.find(std::string(key));
            if (it == params.end()) {
                return defaultValue;
            }
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException{
                    "{} parameter '{}' has invalid type",
                    std::string{className}, std::string(key)};
            }
        }

        // Overload for unordered_map
        template <typename T>
        static T optional(
            const std::unordered_map<std::string, std::any> &params,
            const std::string_view key, const T &defaultValue,
            const std::string_view className) {
            const auto it = params.find(std::string(key));
            if (it == params.end()) {
                return defaultValue;
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
         @brief Retrieve a nested map parameter from a parameter map.
         * @param params The parameter map containing the nested map.
         * @param key The key to look up in the parameter map.
         * @param className The name of the class requesting the parameter.
         * @return A reference to the nested map stored at the given key.
         * @throws PluginException If the key is missing or the type is invalid.
         */
        static const std::map<std::string, std::any> &requireMap(
            const std::map<std::string, std::any> &params,
            const std::string_view key, const std::string_view className) {
            const auto it = params.find(std::string(key));

            if (it == params.end()) {
                throw exception::PluginException("{} requires parameter '{}'",
                                                 std::string(className),
                                                 std::string(key));
            }
            try {
                return std::any_cast<const std::map<std::string, std::any> &>(
                    it->second);
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException(
                    "{} parameter '{}' has invalid type",
                    std::string(className), std::string(key));
            }
        }

        // Overload for unordered_map
        static const std::map<std::string, std::any> &requireMap(
            const std::unordered_map<std::string, std::any> &params,
            const std::string_view key, const std::string_view className) {
            const auto it = params.find(std::string(key));

            if (it == params.end()) {
                throw exception::PluginException("{} requires parameter '{}'",
                                                 std::string(className),
                                                 std::string(key));
            }
            try {
                return std::any_cast<const std::map<std::string, std::any> &>(
                    it->second);
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException(
                    "{} parameter '{}' has invalid type",
                    std::string(className), std::string(key));
            }
        }
    };
}  // namespace raytracer::util
