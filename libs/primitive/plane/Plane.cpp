/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
*/

#include "Plane.hpp"

#include <any>

#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Plane::Plane(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Plane", util::Helpers::toVector(args, "center", "Plane"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Plane")),
          _normal(util::Helpers::toVector(args, "normal", "Plane")) {
        _normal = util::Helpers::normalVector(_normal);
    }

    Plane::Plane(const maths::Vector &vector, const maths::Vector &normal)
        : Plane(nullptr, vector, normal) {
    }

    Plane::Plane(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        const maths::Vector &vector, const maths::Vector &normal)
        : APrimitive("Plane", vector, std::move(material)), _normal(normal) {
        _normal = util::Helpers::normalVector(_normal);
    }

    const maths::Vector &Plane::normal() const noexcept {
        return _normal;
    }

    std::optional<HitContext> Plane::hits(const maths::Ray &ray) {
        const double denom = _normal.dot(ray.direction);
        if (std::abs(denom) < EPS) {
            return std::nullopt;
        }
        const double t = (_center - ray.origin).dot(_normal) / denom;
        if (t < EPS) {
            return std::nullopt;
        }

        const maths::Vector hitPoint = ray.origin + ray.direction * t;

        const maths::Vector &normal = _normal;
        const maths::Vector helper = (std::abs(normal.y) < 0.999)
                                         ? maths::Vector(0, 1, 0)
                                         : maths::Vector(1, 0, 0);
        const maths::Vector uAxis = normal.cross(helper).normalized();
        const maths::Vector vAxis = normal.cross(uAxis).normalized();
        const maths::Vector localHit = hitPoint - _center;
        const double u = localHit.dot(uAxis);
        const double v = localHit.dot(vAxis);

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

    IPrimitive::BoundingBox Plane::boundingBox() {
        // TODO
        return {0, 0, 0, 0, 0, 0};
    }
}  // namespace raytracer::object::primitive
