/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ObjectMiddleware
*/

#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "exception/PluginException.hpp"
#include "math/Vector.hpp"

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
         @brief Validates and retrieves an argument from a vector of std::any
         objects. This function checks if the specified index is within the
         bounds of the vector and attempts to cast the argument at that index to
         the specified type T.
         * @tparam T The expected type of the argument at the specified index.
         * @param args The vector of std::any objects containing the arguments
         to validate.
         * @param index The index of the argument to validate and retrieve from
         the vector.
         * @param className The name of the class for which the argument is
         being validated, used in error messages.
         * @param nbArgs The expected number of arguments for the class, used in
         error messages if the index is out of bounds.
         * @return A reference to the argument of type T if validation and
         casting are successful.
         * @throws PluginException If the index is out of bounds or if the type
         casting fails, with a message indicating the nature of the error.
         */
        template <typename T>
        static const T &validate(const std::vector<std::any> &args,
                                 const std::size_t index,
                                 const std::string_view className,
                                 const std::size_t nbArgs) {
            if (index >= args.size()) {
                throw exception::PluginException{
                    "{} requires at least {} arguments", std::string(className),
                    std::to_string(nbArgs)};
            }
            try {
                return std::any_cast<const T &>(args.at(index));
            } catch (const std::bad_any_cast &) {
                throw exception::PluginException{
                    "{} argument : {} has invalid type", std::string{className},
                    std::to_string(index)};
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
         @brief Check if a double given color vector is negative or not. Because
         a RGB color can't be negative.
         * @param color color vector to check. Compose of r g and b value.
         * @param className name of the class of the value.v
         * @throws PluginException If the index is out of bounds or if the type
         casting fails, with a message indicating the nature of the error.
         */
        static void vectorColor(maths::Vector color,
                                std::string_view className);
    };
}  // namespace raytracer::util
