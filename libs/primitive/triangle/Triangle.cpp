/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"

#include <algorithm>
#include <util/middleware/ObjectMiddleware.hpp>

#include "math/Vector.hpp"
#include "util/middleware/Helpers.hpp"

namespace raytracer::object::primitive {
    Triangle::Triangle(const std::map<std::string, std::any> &args)
        : APrimitive("Triangle",
                     util::Helpers::toVector(args, "center", "Triangle"),
                     util::ObjectMiddleware::validate<
                         std::shared_ptr<material::IMaterial>>(args, "material",
                                                               "Triangle")),
          _v1(util::Helpers::toVector(args, "v1", "Triangle")),
          _v2(util::Helpers::toVector(args, "v2", "Triangle")) {
        const maths::Vector edge1 = _v1 - _center;
        const maths::Vector edge2 = _v2 - _center;

        util::Helpers::notCollinearVector(edge1, edge2, "v1", "v2", "Triangle");
    }

    Triangle::Triangle(const maths::Vector &center, const maths::Vector &v1,
                       const maths::Vector &v2)
        : Triangle(nullptr, center, v1, v2) {
    }

    Triangle::Triangle(std::shared_ptr<material::IMaterial> material,
                       const maths::Vector &center, const maths::Vector &v1,
                       const maths::Vector &v2)
        : APrimitive("Triangle", center, std::move(material)),
          _v1(v1),
          _v2(v2) {
        const maths::Vector edge1 = _v1 - _center;
        const maths::Vector edge2 = _v2 - _center;

        util::Helpers::notCollinearVector(edge1, edge2, "v1", "v2", "Triangle");
    }

    maths::Vector Triangle::v1() const noexcept {
        return _v1;
    }

    maths::Vector Triangle::v2() const noexcept {
        return _v2;
    }

    double Triangle::hits(const maths::Ray &ray) {
        const maths::Vector edge1 = _v1 - _center;
        const maths::Vector edge2 = _v2 - _center;
        const maths::Vector rayCrossE2 = ray.direction.cross(edge2);
        const double det = edge1.dot(rayCrossE2);

        if (std::abs(det) < K_RAY_EPSILON) {
            return -1;  // Ray is parallel to triangle
        }

        const double invDet = 1.0 / det;
        const maths::Vector s = ray.origin - _center;
        const double u = s.dot(rayCrossE2) * invDet;
        if (u < -K_RAY_EPSILON || u - 1 > K_RAY_EPSILON) {
            return -1;  // Ray pass outside edge2 bounds
        }

        const maths::Vector sCrossE1 = s.cross(edge1);
        if (const double v = ray.direction.dot(sCrossE1) * invDet;
            v < -K_RAY_EPSILON || u + v - 1 > K_RAY_EPSILON) {
            return -1;  // Ray pass outside edge1 bounds
        }

        if (const double t = edge2.dot(sCrossE1) * invDet; t < K_RAY_EPSILON) {
            return -1;  // Ray intersection behind the origin
        } else {
            return t;
        }
    }

    IPrimitive::BoundingBox Triangle::boundingBox() {
        const auto v0 = _center;
        const auto v1 = _v1;
        const auto v2 = _v2;
        const auto minX = std::min({v0.x, v1.x, v2.x});
        const auto minY = std::min({v0.y, v1.y, v2.y});
        const auto minZ = std::min({v0.z, v1.z, v2.z});
        const auto maxX = std::max({v0.x, v1.x, v2.x});
        const auto maxY = std::max({v0.y, v1.y, v2.y});
        const auto maxZ = std::max({v0.z, v1.z, v2.z});

        return {.x = minX,
                .y = minY,
                .z = minZ,
                .w = maxX - minX,
                .h = maxY - minY,
                .d = maxZ - minZ};
    }

    SurfaceData Triangle::surfaceData(const maths::Vector &hitPoint) const {
        const auto edge1 = _v1 - _center;
        const auto edge2 = _v2 - _center;
        const auto normal = edge1.cross(edge2).normalized();
        const auto hitVector = hitPoint - _center;
        const double d00 = edge1.dot(edge1);
        const double d01 = edge1.dot(edge2);
        const double d11 = edge2.dot(edge2);
        const double d20 = hitVector.dot(edge1);
        const double d21 = hitVector.dot(edge2);
        const double denom = d00 * d11 - d01 * d01;
        const double u = (d20 * d11 - d21 * d01) / denom;
        const double v = (d21 * d00 - d20 * d01) / denom;

        SurfaceData data{
            .normal = normal, .uv = maths::Vector(u, v, 0), .material = {}};
        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }
        return data;
    }
}  // namespace raytracer::object::primitive
