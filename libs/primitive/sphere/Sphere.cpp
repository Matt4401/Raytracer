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
#include <memory>
#include <string>

#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Sphere::Sphere(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Sphere", util::Helpers::toVector(args, "center", "Sphere"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Sphere")),
          _radius(
              util::ObjectMiddleware::validate<double>(args, "r", "Sphere")) {
        util::Helpers::unsignedDouble(_radius, "r", "Sphere");
    }

    Sphere::Sphere(const maths::Vector &vector, const double radius)
        : Sphere(nullptr, vector, radius) {
    }

    Sphere::Sphere(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        const maths::Vector &vector, const double radius)
        : APrimitive("Sphere", vector, std::move(material)), _radius(radius) {
        util::Helpers::unsignedDouble(_radius, "radius", "Sphere");
    }

    const double &Sphere::radius() const noexcept {
        return _radius;
    }

    std::optional<HitContext> Sphere::hits(const maths::Ray &ray) {
        const maths::Vector oc = ray.origin - _center;
        const maths::Vector ocVec(oc.x, oc.y, oc.z);
        const double a = ray.direction.dot(ray.direction);
        const double b = 2.0 * ocVec.dot(ray.direction);
        const double c = ocVec.dot(ocVec) - _radius * _radius;
        const double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return std::nullopt;
        }

        const double sqrtDiscriminant = std::sqrt(discriminant);
        const double t0 = (-b - sqrtDiscriminant) / (2.0 * a);
        const double t1 = (-b + sqrtDiscriminant) / (2.0 * a);

        double t = -1.0;
        if (t0 > K_RAY_EPSILON) {
            t = t0;
        } else if (t1 > K_RAY_EPSILON) {
            t = t1;
        } else {
            return std::nullopt;
        }

        const maths::Vector hitPoint = ray.origin + ray.direction * t;

        const maths::Vector normal = (hitPoint - _center).normalized();
        const double u = 0.5 + std::atan2(normal.z, normal.x) / (2 * M_PI);
        const double v = 0.5 - std::asin(normal.y) / M_PI;

        SurfaceData surfData{
            .normal = normal, .uv = maths::Vector(u, v, 0), .material = {}};

        if (this->_material) {
            surfData.material = this->_material->evaluate(surfData, hitPoint);
        }

        return HitContext{
            .distance = t,
            .hitPoint = hitPoint,
            .surfaceData = surfData};
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
