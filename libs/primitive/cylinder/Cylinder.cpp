/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder
*/

#include "Cylinder.hpp"

#include <algorithm>
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
    Cylinder::Cylinder(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Cylinder", util::Helpers::toVector(args, "center", "Cylinder"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Cylinder")),
          _radius(util::ObjectMiddleware::validate<double>(args, "radius",
                                                           "Cylinder")) {
        util::Helpers::unsignedDouble(_radius, "radius", "Cylinder");
    }

    Cylinder::Cylinder(const maths::Vector &center, const double radius)
        : Cylinder(nullptr, center, radius) {
    }

    Cylinder::Cylinder(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        const maths::Vector &center, const double radius)
        : APrimitive("Cylinder", center, std::move(material)), _radius(radius) {
        util::Helpers::unsignedDouble(_radius, "radius", "Cylinder");
    }

    const double &Cylinder::radius() const noexcept {
        return _radius;
    }

    bool Cylinder::hits(const maths::Ray &ray, HitRecord &record) const {
        const maths::Vector oc = ray.origin - _center;
        const double a = ray.direction.x * ray.direction.x +
                         ray.direction.z * ray.direction.z;

        if (std::abs(a) < K_RAY_EPSILON) {
            return false;
        }
        const double b =
            2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
        const double c = oc.x * oc.x + oc.z * oc.z - _radius * _radius;
        const double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return false;
        }

        const double sqrtDiscriminant = std::sqrt(discriminant);
        double t0 = (-b - sqrtDiscriminant) / (2.0 * a);
        double t1 = (-b + sqrtDiscriminant) / (2.0 * a);

        if (t0 > t1)
            std::swap(t0, t1);

        for (const double t : {t0, t1}) {
            if (t <= K_RAY_EPSILON)
                continue;
            const maths::Vector impact = ray.origin + ray.direction * t;
            if (!validateConstraints(impact))
                continue;
            record.t = t;
            record.objectId = id();
            record.hitPoint = impact;
            return true;
        }
        return false;
    }

    IPrimitive::AABoundingBox Cylinder::boundingBox() {
        if (!_hasLimit) {
            return {-1e6, -1e6, _center.z - _radius, 2e6, 2e6, 2 * _radius};
        }
        return _sceneLimit;
    }

    SurfaceData Cylinder::surfaceData(const HitRecord &record) const {
        const maths::Vector &hitPoint = record.hitPoint;
        const maths::Vector normal =
            maths::Vector(hitPoint.x - _center.x, 0, hitPoint.z - _center.z)
                .normalized();
        SurfaceData data{
            .normal = normal, .uv = maths::Vector(0, 0, 0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }

        return data;
    }

}  // namespace raytracer::object::primitive
