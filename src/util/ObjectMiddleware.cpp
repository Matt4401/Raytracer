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

    void ObjectMiddleware::vectorColor(maths::Vector color,
                                       const std::string_view className) {
        if (color.x < 0 || color.y < 0 || color.z < 0) {
            throw exception::PluginException{
                "{} color can't have negative value : {} {} {}", className,
                color.x, color.y, color.z};
        }
    }
}  // namespace raytracer::util
