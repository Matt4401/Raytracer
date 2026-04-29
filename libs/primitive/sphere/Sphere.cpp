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

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Sphere::Sphere(const std::vector<std::any> &args)
        : APrimitive("Sphere",
                     util::ObjectMiddleware::validate<maths::Vector>(
                         args, 0, "Sphere", EXPECTED_ARGS)),
          _radius(util::ObjectMiddleware::validate<double>(args, 1, "Sphere",
                                                           EXPECTED_ARGS)) {
        util::ObjectMiddleware::unsignedDouble(_radius, "radius", "Sphere");
    }

    Sphere::Sphere(const maths::Vector &vector, const double radius)
        : APrimitive("Sphere", vector), _radius(radius) {
        util::ObjectMiddleware::unsignedDouble(_radius, "radius", "Sphere");
    }

    const double &Sphere::radius() const noexcept {
        return _radius;
    }

    double Sphere::hits(const maths::Ray &ray) {
        const maths::Vector op = _center - ray.origin;
        const double b = op.dot(ray.direction);
        const double a = ray.direction.dot(ray.direction);
        const double c = op.dot(op) - _radius * _radius;
        double det = b * b - 4 * a * c;
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

    SurfaceData Sphere::surfaceData(const maths::Vector& hitPoint) const {
        maths::Vector normal = (hitPoint - _center).normalized();

        double u = 0.5 + std::atan2(normal.z, normal.x) / (2 * M_PI);
        double v = 0.5 - std::asin(normal.y) / M_PI;

        return {
            .normal = normal,
            .uv = maths::Vector(u, v, 0),
            .color = maths::Color(255, 255, 255),
            .emission = maths::Vector(0, 0, 0),
            .reflType = RefltT::DIFF,
            .extraParams = {}
        };
    }
}  // namespace raytracer::object::primitive
