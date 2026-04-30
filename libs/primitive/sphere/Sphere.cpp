/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <any>
#include <memory>
#include <cmath>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Sphere::Sphere(const std::map<std::string, std::any> &params)
        : APrimitive("Sphere",
                     util::Helpers::toVector(params, "center", "Sphere"),
                     nullptr),
          _radius(util::ObjectMiddleware::validate<double>(params, "r", "Sphere")) {
        util::Helpers::unsignedDouble(_radius, "r", "Sphere");
    }

    Sphere::Sphere(const maths::Vector &vector, const double radius)
        : Sphere(nullptr, vector, radius) {}

    Sphere::Sphere(std::shared_ptr<raytracer::object::material::IMaterial> material,
                   const maths::Vector &vector, const double radius)
        : APrimitive("Sphere", vector, std::move(material)), _radius(radius) {
        util::Helpers::unsignedDouble(_radius, "radius", "Sphere");
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

    SurfaceData Sphere::surfaceData(const maths::Vector& hitPoint) const {
        maths::Vector normal = (hitPoint - _center).normalized();

        double u = 0.5 + std::atan2(normal.z, normal.x) / (2 * M_PI);
        double v = 0.5 - std::asin(normal.y) / M_PI;

        SurfaceData data{
            .normal = normal,
            .uv = maths::Vector(u, v, 0),
            .color = maths::Color(255, 255, 255),
            .emission = maths::Vector(0, 0, 0),
            .reflType = RefltT::DIFF,
            .extraParams = {}
        };

        if (this->_material) {
            this->_material->apply(data, hitPoint);
        }

        return data;
    }
}  // namespace raytracer::object::primitive
