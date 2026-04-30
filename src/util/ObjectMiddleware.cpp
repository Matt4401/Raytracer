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
}  // namespace raytracer::util
