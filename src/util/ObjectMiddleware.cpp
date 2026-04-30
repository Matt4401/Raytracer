/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ObjectMiddleware
*/

#include "util/ObjectMiddleware.hpp"

#include "exception/PluginException.hpp"

namespace raytracer::util {
    void ObjectMiddleware::unsignedDouble(const double value,
                                          const std::string_view fieldName,
                                          const std::string_view className) {
        if (value < 0) {
            throw exception::PluginException{"{} {} can't be negative: {}",
                                             className, fieldName, value};
        }
    }

    maths::Color toColor(const std::map<std::string, std::any> &params,
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

    maths::Vector toVector(const std::map<std::string, std::any> &params,
                           const std::string_view keyName,
                           const std::string_view className) {
        const auto &vectorMap =
            ObjectMiddleware::requireMap(params, keyName, className);

        return maths::Vector(
            ObjectMiddleware::validate<double>(vectorMap, "x", className),
            ObjectMiddleware::validate<double>(vectorMap, "y", className),
            ObjectMiddleware::validate<double>(vectorMap, "z", className));
    }
}  // namespace raytracer::util
