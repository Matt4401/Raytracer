/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MapConverter
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
#include "util/ObjectMiddleware.hpp"

namespace raytracer::util {
    template <typename T>
    T mapTo(const std::map<std::string, std::any> &mapData,
            const std::string_view className) {
        if constexpr (std::is_same_v<T, maths::Vector>) {
            return maths::Vector(
                ObjectMiddleware::validate<double>(mapData, "x", className),
                ObjectMiddleware::validate<double>(mapData, "y", className),
                ObjectMiddleware::validate<double>(mapData, "z", className));
        } else if constexpr (std::is_same_v<T, maths::Color>) {
            return maths::Color(
                static_cast<unsigned char>(
                    ObjectMiddleware::validate<int>(mapData, "r", className)),
                static_cast<unsigned char>(
                    ObjectMiddleware::validate<int>(mapData, "g", className)),
                static_cast<unsigned char>(
                    ObjectMiddleware::validate<int>(mapData, "b", className)));
        }
        throw exception::PluginException{
            "{} mapTo conversion is not supported for this type",
            std::string(className)};
    }
}  // namespace raytracer::util
