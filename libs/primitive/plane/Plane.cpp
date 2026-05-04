/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
*/

#include "Plane.hpp"

#include <any>
#include <vector>

#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    Plane::Plane(const std::vector<std::any>& args) {
        if (args.size() != EXPECTED_ARGS) {
            throw std::invalid_argument(
                "Plane requires exactly " + std::to_string(EXPECTED_ARGS) +
                " arguments, but " + std::to_string(args.size()) +
                " were provided.");
        }
    }
}  // namespace raytracer::object::primitive
