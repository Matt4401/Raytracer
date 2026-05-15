/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LimitedCylinder
*/

#include "LimitedCylinder.hpp"

#include <any>
#include <cmath>
#include <map>
#include <memory>
#include <string>

#include "math/Vector.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    LimitedCylinder::LimitedCylinder(
        const std::map<std::string, std::any> &args)
        : Cylinder(args) {
        _height = util::ObjectMiddleware::validate<double>(args, "height",
                                                           "LimitedCylinder");
        _closed = util::ObjectMiddleware::optional<bool>(args, "closed", false,
                                                         "LimitedCylinder");
        util::Helpers::unsignedDouble(_height, "height", "LimitedCylinder");
    }

    LimitedCylinder::LimitedCylinder(const maths::Vector &center,
                                     const double radius, const double height,
                                     const bool closed)
        : Cylinder(nullptr, center, radius), _height(height), _closed(closed) {
        util::Helpers::unsignedDouble(_height, "height", "LimitedCylinder");
    }

    bool LimitedCylinder::validateConstraints(const maths::Vector &p) const {
        const double minY = _center.y - _height / 2.0;
        const double maxY = _center.y + _height / 2.0;

        return (p.y >= minY - K_RAY_EPSILON && p.y <= maxY + K_RAY_EPSILON);
    }

    bool LimitedCylinder::hits(const maths::Ray &ray, HitRecord &record) const {
        HitRecord best{};
        bool found = false;

        if (HitRecord side{}; Cylinder::hits(ray, side)) {
            best = side;
            found = true;
        }
        if (_closed && std::abs(ray.direction.y) > K_RAY_EPSILON) {
            const double minY = _center.y - _height / 2.0;

            for (const double maxY = _center.y + _height / 2.0;
                 const double yCap : {minY, maxY}) {
                const double t = (yCap - ray.origin.y) / ray.direction.y;
                if (t <= K_RAY_EPSILON) {
                    continue;
                }
                const maths::Vector p = ray.origin + ray.direction * t;
                const double dx = p.x - _center.x;

                if (const double dz = p.z - _center.z;
                    dx * dx + dz * dz > radius() * radius() + K_RAY_EPSILON) {
                    continue;
                }
                if (!found || t < best.t) {
                    best.t = t;
                    best.objectId = id();
                    best.hitPoint = p;
                    found = true;
                }
            }
        }
        if (!found) {
            return false;
        }
        record = best;
        return true;
    }

    SurfaceData LimitedCylinder::surfaceData(const HitRecord &record) const {
        const double minY = _center.y - _height / 2.0;
        const double maxY = _center.y + _height / 2.0;
        const maths::Vector &p = record.hitPoint;

        if (_closed && std::abs(p.y - maxY) <= K_RAY_EPSILON) {
            SurfaceData data{.normal = maths::Vector(0, 1, 0),
                             .uv = maths::Vector(0, 0, 0),
                             .material = {}};
            if (this->_material) {
                data.material = this->_material->evaluate(data, p);
            }
            return data;
        }
        if (_closed && std::abs(p.y - minY) <= K_RAY_EPSILON) {
            SurfaceData data{.normal = maths::Vector(0, -1, 0),
                             .uv = maths::Vector(0, 0, 0),
                             .material = {}};
            if (this->_material) {
                data.material = this->_material->evaluate(data, p);
            }
            return data;
        }
        return Cylinder::surfaceData(record);
    }
}  // namespace raytracer::object::primitive
