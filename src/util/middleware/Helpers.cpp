/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Helpers
*/

#include "util/middleware/Helpers.hpp"

#include <any>
#include <map>
#include <string>
#include <string_view>

#include "exception/PluginException.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::util {

    void Helpers::unsignedInt(const int value, const std::string_view fieldName,
                              const std::string_view className) {
        if (value < 0) {
            throw exception::PluginException{"{} {} can't be negative: {}",
                                             className, fieldName, value};
        }
    }

    void Helpers::unsignedDouble(const double value,
                                 const std::string_view fieldName,
                                 const std::string_view className) {
        if (value < 0) {
            throw exception::PluginException{"{} {} can't be negative: {}",
                                             className, fieldName, value};
        }
    }

    maths::Vector Helpers::normalVector(maths::Vector &normal) {
        if (normal == maths::Vector(0, 0, 0)) {
            throw exception::PluginException{
                "Normal vector cannot be the zero"};
        }
        normal.normalize();
        return normal;
    }

    maths::Color Helpers::toColor(const std::map<std::string, std::any> &params,
                                  const std::string_view keyName,
                                  const std::string_view className) {
        const auto &colorMap =
            ObjectMiddleware::requireMap(params, keyName, className);

        return maths::Color(
            ObjectMiddleware::validate<unsigned char>(colorMap, "r", className),
            ObjectMiddleware::validate<unsigned char>(colorMap, "g", className),
            ObjectMiddleware::validate<unsigned char>(colorMap, "b",
                                                      className));
    }

    maths::Vector Helpers::toVector(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const std::string_view className) {
        const auto &vectorMap =
            ObjectMiddleware::requireMap(params, keyName, className);

        return maths::Vector(
            ObjectMiddleware::validate<double>(vectorMap, "x", className),
            ObjectMiddleware::validate<double>(vectorMap, "y", className),
            ObjectMiddleware::validate<double>(vectorMap, "z", className));
    }

    maths::Color Helpers::optionalColor(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const maths::Color &defaultValue,
        const std::string_view className) {
        if (!params.contains(std::string(keyName))) {
            return defaultValue;
        }
        return toColor(params, keyName, className);
    }

    maths::Vector Helpers::optionalVector(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const maths::Vector &defaultValue,
        const std::string_view className) {
        if (!params.contains(std::string(keyName))) {
            return defaultValue;
        }
        return toVector(params, keyName, className);
    }
}  // namespace raytracer::util
