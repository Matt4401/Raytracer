/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <any>
#include <cmath>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Sphere::Sphere(const std::map<std::string, std::any> &params)
        : APrimitive("Sphere",
                     util::ObjectMiddleware::validate<maths::Vector>(
                         params, "pos", "Sphere"),
                     maths::Vector(0, 0, 0),
                     util::ObjectMiddleware::validate<maths::Color>(
                         params, "color", "Sphere"),
                     RefltT::DIFF),
          _radius(
              util::ObjectMiddleware::validate<double>(params, "r", "Sphere")) {
        util::ObjectMiddleware::unsignedDouble(_radius, "r", "Sphere");
        util::ObjectMiddleware::color(_color, "Sphere");
    }

    Sphere::Sphere(const maths::Vector &vector, const maths::Vector &emission,
                   const maths::Color &color, const double radius,
                   const RefltT refl)
        : APrimitive("Sphere", vector, emission, color, refl), _radius(radius) {
        util::ObjectMiddleware::unsignedDouble(_radius, "radius", "Sphere");
        util::ObjectMiddleware::color(_color, "Sphere");
    }

    const double &Sphere::radius() const noexcept {
        return _radius;
    }

    double Sphere::hits(const maths::Ray &ray) {
        const maths::Vector op = _center - ray.origin;
        const double b = op.dot(ray.direction);
        const double a = ray.direction.dot(ray.direction);
        const double c = op.dot(op) - _radius * _radius;
        const double det = b * b - 4 * a * c;
        if (det < 0) {
            return 0;
        }
        const double sqrtDiscriminant = std::sqrt(det);
        const double t0 = (-b - sqrtDiscriminant) / (2.0 * a);
        const double t1 = (-b + sqrtDiscriminant) / (2.0 * a);

        if (t0 > EPS) {
            return t0;
        }
        if (t1 > EPS) {
            return t1;
        }
        return 0;
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
