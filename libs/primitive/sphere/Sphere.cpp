/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <any>
#include <cmath>
#include <vector>

#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Sphere::Sphere(const std::vector<std::any> &args)
        : APrimitive("Sphere",
                     util::ObjectMiddleware::validate<maths::Vector>(
                         args, 0, "Sphere", EXPECTED_ARGS),
                     util::ObjectMiddleware::validate<maths::Vector>(
                         args, 1, "Sphere", EXPECTED_ARGS),
                     util::ObjectMiddleware::validate<maths::Vector>(
                         args, 2, "Sphere", EXPECTED_ARGS),
                     util::ObjectMiddleware::validate<RefltT>(args, 4, "Sphere",
                                                              EXPECTED_ARGS)),
          _radius(util::ObjectMiddleware::validate<double>(args, 3, "Sphere",
                                                           EXPECTED_ARGS)) {
        util::ObjectMiddleware::unsignedDouble(_radius, "radius", "Sphere");
        util::ObjectMiddleware::vectorColor(_color, "Sphere");
    }

    Sphere::Sphere(const maths::Vector &vector, const maths::Vector &emission,
                   const maths::Vector &color, const double radius,
                   const RefltT refl)
        : APrimitive("Sphere", vector, emission, color, refl), _radius(radius) {
        util::ObjectMiddleware::unsignedDouble(_radius, "radius", "Sphere");
        util::ObjectMiddleware::vectorColor(_color, "Sphere");
    }

    const double &Sphere::radius() const noexcept {
        return _radius;
    }

    double Sphere::hits(const maths::Ray &ray) {
        const maths::Vector op = _center - ray.origin;
        double t;
        constexpr double EPS = 1e-4;
        const double b = op.dot(ray.direction);
        double det = b * b - op.dot(op) + _radius * _radius;
        if (det < 0) {
            return 0;
        }
        det = std::sqrt(det);
        return (t = b - det) > EPS ? t : ((t = b + det) > EPS ? t : 0);
    }

    IPrimitive::BoundingBox Sphere::boundingBox() {
        return {
            .x = _center.x - _radius,
            .y = _center.y - _radius,
            .z = _center.z - _radius,
            .w = 2 * _radius,
            .h = 2 * _radius,
            .d = 2 * _radius,
        };
    }
}  // namespace raytracer::object::primitive
