/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone
*/

#include "Cone.hpp"

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
    Cone::Cone(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Cone", util::Helpers::toVector(args, "center", "Cone"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Cone")),
          _openingFactor(util::ObjectMiddleware::optional<double>(
              args, "openingFactor", 1.0, "Cone")) {
        util::Helpers::unsignedDouble(_openingFactor, "openingFactor", "Cone");
    }

    Cone::Cone(const maths::Vector &center, const double openingFactor)
        : Cone(nullptr, center, openingFactor) {
    }

    Cone::Cone(std::shared_ptr<raytracer::object::material::IMaterial> material,
               const maths::Vector &center, const double openingFactor)
        : APrimitive("Cone", center, std::move(material)),
          _openingFactor(openingFactor) {
        util::Helpers::unsignedDouble(_openingFactor, "openingFactor", "Cone");
    }

    const double &Cone::openingFactor() const noexcept {
        return _openingFactor;
    }

    bool Cone::hits(const maths::Ray &ray, HitRecord &record) const {
        return solveQuadraticIntersection(ray, record);
    }

    bool Cone::solveQuadraticIntersection(const maths::Ray &ray,
                                          HitRecord &record) const {
        const maths::Vector oc = ray.origin - _center;
        const double a = ray.direction.x * ray.direction.x +
                         ray.direction.z * ray.direction.z -
                         _openingFactor * ray.direction.y * ray.direction.y;
        if (std::abs(a) < K_RAY_EPSILON) {
            return false;
        }

        const double b =
            2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z -
                   _openingFactor * oc.y * ray.direction.y);
        const double c =
            oc.x * oc.x + oc.z * oc.z - _openingFactor * oc.y * oc.y;
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

    IPrimitive::AABoundingBox Cone::boundingBox() {
        if (!_hasLimit) {
            return INFINITE_BOUNDING_BOX;
        }
        return _sceneLimit;
    }

    SurfaceData Cone::surfaceData(const HitRecord &record) const {
        const maths::Vector &p = record.hitPoint;
        const auto grad = maths::Vector(2 * (p.x - _center.x),
                                        -2 * _openingFactor * (p.y - _center.y),
                                        2 * (p.z - _center.z));
        const maths::Vector normal = grad.normalized();
        SurfaceData data{
            .normal = normal, .uv = maths::Vector(0, 0, 0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, p);
        }
        return data;
    }
}  // namespace raytracer::object::primitive
