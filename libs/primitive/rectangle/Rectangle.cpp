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
          _maxPoint(util::Helpers::toVector(args, "maxPoint", "Rectangle")) {
        util::Helpers::notCollinearVector(_maxPoint, _center, "maxPoint",
                                          "center", "Rectangle");
    }

    Rectangle::Rectangle(const maths::Vector &center,
                         const maths::Vector &maxPoint)
        : Rectangle(nullptr, center, maxPoint) {
    }

    Rectangle::Rectangle(std::shared_ptr<material::IMaterial> material,
                         const maths::Vector &center,
                         const maths::Vector &maxPoint)
        : APrimitive("Rectangle", center, std::move(material)),
          _maxPoint(maxPoint) {
        util::Helpers::notCollinearVector(_maxPoint, _center, "maxPoint",
                                          "center", "Rectangle");
    }

    maths::Vector Rectangle::maxPoint() const {
        return _maxPoint;
    }

    bool Rectangle::hits(const maths::Ray &ray, HitRecord &record) const {
        const double k = _center.z;

        if (std::abs(ray.direction.z) < 1e-9)
            return false;

        const double t = (k - ray.origin.z) / ray.direction.z;
        if (t < 0.001)
            return false;

        const maths::Vector hitPoint = ray.origin + ray.direction * t;
        const double minX = std::min(_center.x, _maxPoint.x);
        const double maxX = std::max(_center.x, _maxPoint.x);
        const double minY = std::min(_center.y, _maxPoint.y);

        if (const double maxY = std::max(_center.y, _maxPoint.y);
            hitPoint.x < minX || hitPoint.x > maxX || hitPoint.y < minY ||
            hitPoint.y > maxY) {
            return false;
        }
        record.t = t;
        record.objectId = id();
        return true;
    }

    IPrimitive::AABoundingBox Rectangle::boundingBox() {
        const auto minX = std::min(_center.x, _maxPoint.x);
        const auto minY = std::min(_center.y, _maxPoint.y);
        const auto minZ = std::min(_center.z, _maxPoint.z);
        const auto maxX = std::max(_center.x, _maxPoint.x);
        const auto maxY = std::max(_center.y, _maxPoint.y);
        const auto maxZ = std::max(_center.z, _maxPoint.z);

        return maths::AABoundingBox{.x = minX,
                                    .y = minY,
                                    .z = minZ,
                                    .w = maxX - minX,
                                    .h = maxY - minY,
                                    .d = maxZ - minZ};
    }

    SurfaceData Rectangle::surfaceData(const maths::Vector &hitPoint) const {
        const auto normal = (_maxPoint - _center).normalized();
        SurfaceData data{
            .normal = normal, .uv = maths::Vector(0, 0, 0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }
        return data;
    }
}  // namespace raytracer::object::primitive