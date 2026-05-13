/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Rectangle
*/

#include "Rectangle.hpp"

#include <any>
#include <map>
#include <memory>
#include <string>
#include <util/middleware/ObjectMiddleware.hpp>

#include "object/material/IMaterial.hpp"
#include "object/primitive/APrimitive.hpp"
#include "util/middleware/Helpers.hpp"

namespace raytracer::object::primitive {
    Rectangle::Rectangle(const std::map<std::string, std::any> &args)
        : APrimitive("Rectangle",
                     util::Helpers::toVector(args, "center", "Rectangle"),
                     util::ObjectMiddleware::validate<
                         std::shared_ptr<material::IMaterial>>(args, "material",
                                                               "Rectangle")),
          _maxPoint(util::Helpers::toVector(args, "maxPoint", "Rectangle")),
          _normal(util::Helpers::toVector(args, "normal", "Rectangle")) {
        _normal = util::Helpers::normalVector(_normal);
        util::Helpers::notCollinearVector(_maxPoint, _center, "maxPoint",
                                          "center", "Rectangle");
    }

    Rectangle::Rectangle(const maths::Vector &center,
                         const maths::Vector &maxPoint,
                         const maths::Vector &normal)
        : Rectangle(nullptr, center, maxPoint, normal) {
    }

    Rectangle::Rectangle(std::shared_ptr<material::IMaterial> material,
                         const maths::Vector &center,
                         const maths::Vector &maxPoint,
                         const maths::Vector &normal)
        : APrimitive("Rectangle", center, std::move(material)),
          _maxPoint(maxPoint),
          _normal(normal) {
        _normal = util::Helpers::normalVector(_normal);
        util::Helpers::notCollinearVector(_maxPoint, _center, "maxPoint",
                                          "center", "Rectangle");
    }

    maths::Vector Rectangle::maxPoint() const {
        return _maxPoint;
    }

    bool Rectangle::verifyHitPoint(const maths::Vector &minP,
                                   const maths::Vector &maxP,
                                   const maths::Vector &normal,
                                   const maths::Vector &hitPoint) {
        const double absNx = std::abs(normal.x);
        const double absNy = std::abs(normal.y);
        const double absNz = std::abs(normal.z);

        if (absNx >= absNy && absNx >= absNz) {
            return hitPoint.y >= std::min(minP.y, maxP.y) &&
                   hitPoint.y <= std::max(minP.y, maxP.y) &&
                   hitPoint.z >= std::min(minP.z, maxP.z) &&
                   hitPoint.z <= std::max(minP.z, maxP.z);
        }
        if (absNy >= absNx && absNy >= absNz) {
            return hitPoint.x >= std::min(minP.x, maxP.x) &&
                   hitPoint.x <= std::max(minP.x, maxP.x) &&
                   hitPoint.z >= std::min(minP.z, maxP.z) &&
                   hitPoint.z <= std::max(minP.z, maxP.z);
        }
        return hitPoint.x >= std::min(minP.x, maxP.x) &&
               hitPoint.x <= std::max(minP.x, maxP.x) &&
               hitPoint.y >= std::min(minP.y, maxP.y) &&
               hitPoint.y <= std::max(minP.y, maxP.y);
    }

    bool Rectangle::hits(const maths::Ray &ray, HitRecord &record) const {
        const double denom = _normal.dot(ray.direction);
        if (std::abs(denom) < EPS)
            return false;

        const double t = (_center - ray.origin).dot(_normal) / denom;
        if (t < EPS)
            return false;

        if (const maths::Vector hitPoint = ray.origin + ray.direction * t;
            !verifyHitPoint(_center, _maxPoint, _normal, hitPoint))
            return false;
        record.t = t;
        record.hitPoint = ray.origin + ray.direction * t;
        record.objectId = id();
        return true;
    }

    IPrimitive::AABoundingBox Rectangle::boundingBox() {
        auto minX = std::min(_center.x, _maxPoint.x);
        auto minY = std::min(_center.y, _maxPoint.y);
        auto minZ = std::min(_center.z, _maxPoint.z);
        auto maxX = std::max(_center.x, _maxPoint.x);
        auto maxY = std::max(_center.y, _maxPoint.y);
        auto maxZ = std::max(_center.z, _maxPoint.z);
        const double absNx = std::abs(_normal.x);
        const double absNy = std::abs(_normal.y);

        if (const double absNz = std::abs(_normal.z);
            absNx >= absNy && absNx >= absNz && (maxX - minX) < EPS) {
            minX -= EPS;
            maxX += EPS;
        } else if (absNy >= absNx && absNy >= absNz && (maxY - minY) < EPS) {
            minY -= EPS;
            maxY += EPS;
        } else if ((maxZ - minZ) < EPS) {
            minZ -= EPS;
            maxZ += EPS;
        }
        return maths::AABoundingBox{.x = minX,
                                    .y = minY,
                                    .z = minZ,
                                    .w = maxX - minX,
                                    .h = maxY - minY,
                                    .d = maxZ - minZ};
    }

    SurfaceData Rectangle::surfaceData(const HitRecord &record) const {
        const maths::Vector &normal = _normal;
        const maths::Vector helper = (std::abs(normal.y) < 0.999)
                                         ? maths::Vector(0, 1, 0)
                                         : maths::Vector(1, 0, 0);
        const maths::Vector uAxis = normal.cross(helper).normalized();
        const maths::Vector vAxis = normal.cross(uAxis).normalized();
        const maths::Vector localHit = record.hitPoint - _center;
        const double u = localHit.dot(uAxis);
        const double v = localHit.dot(vAxis);

        SurfaceData data{
            .normal = normal, .uv = maths::Vector(u, v, 0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, record.hitPoint);
        }
        return data;
    }
}  // namespace raytracer::object::primitive