/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LimitedCone
*/

#include "LimitedCone.hpp"

#include <any>
#include <cmath>
#include <map>
#include <memory>
#include <string>

#include "math/Vector.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    LimitedCone::LimitedCone(const std::map<std::string, std::any> &args)
        : Cone(args) {
        _height = util::ObjectMiddleware::validate<double>(args, "height",
                                                           "LimitedCone");
        _closed = util::ObjectMiddleware::optional<bool>(args, "closed", false,
                                                         "LimitedCone");
        util::Helpers::unsignedDouble(_height, "height", "LimitedCone");
    }

    LimitedCone::LimitedCone(const maths::Vector &center, const double k,
                             const double height, const bool closed)
        : Cone(nullptr, center, k), _height(height), _closed(closed) {
        util::Helpers::unsignedDouble(_height, "height", "LimitedCone");
    }

    bool LimitedCone::validateConstraints(const maths::Vector &p) const {
        const double minY = _center.y - _height / 2.0;
        const double maxY = _center.y + _height / 2.0;

        return (p.y >= minY - K_RAY_EPSILON && p.y <= maxY + K_RAY_EPSILON);
    }

    bool LimitedCone::hits(const maths::Ray &ray, HitRecord &record) const {
        HitRecord best{};
        bool found = false;

        if (solveQuadraticIntersection(ray, best)) {
            found = true;
        }
        if (_closed && std::abs(ray.direction.y) > K_RAY_EPSILON) {
            const double minY = _center.y - _height / 2.0;
            const double maxY = _center.y + _height / 2.0;
            HitRecord capRecord{};

            if (testCapIntersection(ray, minY, capRecord) &&
                (!found || capRecord.t < best.t)) {
                best = capRecord;
                found = true;
            }
            if (testCapIntersection(ray, maxY, capRecord) &&
                (!found || capRecord.t < best.t)) {
                best = capRecord;
                found = true;
            }
        }
        if (!found) {
            return false;
        }
        record = best;
        return true;
    }

    bool LimitedCone::testCapIntersection(const maths::Ray &ray,
                                          const double yCap,
                                          HitRecord &hitRecord) const {
        if (std::abs(ray.direction.y) <= K_RAY_EPSILON) {
            return false;
        }
        const double t = (yCap - ray.origin.y) / ray.direction.y;
        if (t <= K_RAY_EPSILON) {
            return false;
        }

        const maths::Vector p = ray.origin + ray.direction * t;
        const double localY = p.y - _center.y;
        const double capRadiusSq = k() * localY * localY;
        const double dx = p.x - _center.x;

        if (const double dz = p.z - _center.z;
            dx * dx + dz * dz > capRadiusSq + K_RAY_EPSILON) {
            return false;
        }
        hitRecord.t = t;
        hitRecord.objectId = id();
        hitRecord.hitPoint = p;
        return true;
    }

    SurfaceData LimitedCone::surfaceData(const HitRecord &record) const {
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
        return Cone::surfaceData(record);
    }
}  // namespace raytracer::object::primitive
