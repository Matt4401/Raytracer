/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"

#include <util/middleware/ObjectMiddleware.hpp>

#include "../sphere/Sphere.hpp"
#include "math/Vector.hpp"
#include "util/middleware/Helpers.hpp"

namespace raytracer::object::primitive {
    Triangle::Triangle(const std::map<std::string, std::any> &args)
        : APrimitive("Triangle",
                     util::Helpers::toVector(args, "center", "Triangle"),
                     util::ObjectMiddleware::validate<
                         std::shared_ptr<material::IMaterial>>(args, "material",
                                                               "Triangle")),
          _v1(util::Helpers::toVector(args, "v1", "Triangle")),
          _v2(util::Helpers::toVector(args, "v2", "Triangle")) {
    }

    Triangle::Triangle(const maths::Vector &center, const maths::Vector &v1,
                       const maths::Vector &v2)
        : Triangle(nullptr, center, v1, v2) {
    }

    Triangle::Triangle(std::shared_ptr<material::IMaterial> material,
                       const maths::Vector &center, const maths::Vector &v1,
                       const maths::Vector &v2)
        : APrimitive("Triangle", center, std::move(material)),
          _v1(v1),
          _v2(v2) {

    }
}  // namespace raytracer::object::primitive
