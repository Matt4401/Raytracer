/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Face
*/

#include "Face.hpp"

#include <algorithm>
#include <any>
#include <cmath>

namespace raytracer::object::primitive {
    Face::Face(std::size_t faceIndex, const ObjLoader::Face &face,
               const maths::Vector &v0, const maths::Vector &v1,
               const maths::Vector &v2,
               std::shared_ptr<raytracer::object::material::IMaterial> material,
               const std::string &materialName)
        : APrimitive("Face", (v0 + v1 + v2) / 3.0, std::move(material)),
          _face(face),
          _index(faceIndex),
          _materialName(materialName),
          _v0(v0),
          _v1(v1),
          _v2(v2) {
    }

    bool Face::hits(const maths::Ray &ray, HitRecord &record) const {
        const maths::Vector edge1 = _v1 - _v0;
        const maths::Vector edge2 = _v2 - _v0;
        const maths::Vector h = ray.direction.cross(edge2);
        const double a = edge1.dot(h);

        if (std::abs(a) < K_RAY_EPSILON) {
            return false;
        }

        const double f = 1.0 / a;
        const maths::Vector s = ray.origin - _v0;
        const double u = f * s.dot(h);
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        const maths::Vector q = s.cross(edge1);
        const double v = f * ray.direction.dot(q);
        if (v < 0.0 || u + v > 1.0) {
            return false;
        }

        const double t = f * edge2.dot(q);
        if (t <= K_RAY_EPSILON) {
            return false;
        }

        record.t = t;
        record.objectId = getId();
        return true;
    }

    IPrimitive::AABoundingBox Face::boundingBox() {
        const double minX = std::min({_v0.x, _v1.x, _v2.x});
        const double minY = std::min({_v0.y, _v1.y, _v2.y});
        const double minZ = std::min({_v0.z, _v1.z, _v2.z});
        const double maxX = std::max({_v0.x, _v1.x, _v2.x});
        const double maxY = std::max({_v0.y, _v1.y, _v2.y});
        const double maxZ = std::max({_v0.z, _v1.z, _v2.z});

        return {minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ};
    }

    SurfaceData Face::surfaceData(const maths::Vector &hitPoint) const {
        const maths::Vector edge1 = _v1 - _v0;
        const maths::Vector edge2 = _v2 - _v0;
        maths::Vector normal = edge1.cross(edge2).normalized();

        const maths::Vector localHit = hitPoint - _v0;
        const double d00 = edge1.dot(edge1);
        const double d01 = edge1.dot(edge2);
        const double d11 = edge2.dot(edge2);
        const double d20 = localHit.dot(edge1);
        const double d21 = localHit.dot(edge2);
        const double denom = d00 * d11 - d01 * d01;

        double u = 0.0;
        double v = 0.0;
        if (std::abs(denom) > K_RAY_EPSILON) {
            u = (d20 * d11 - d21 * d01) / denom;
            v = (d21 * d00 - d20 * d01) / denom;
        }

        SurfaceData data{.normal = normal,
                         .uv = maths::Vector(u, v, 0),
                         .extraParams = {},
                         .material = {}};
        if (!_materialName.empty()) {
            data.extraParams["materialName"] = std::any(_materialName);
        }
        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }
        return data;
    }

    const ObjLoader::Face &Face::geometry() const noexcept {
        return _face;
    }

    std::size_t Face::index() const noexcept {
        return _index;
    }
}  // namespace raytracer::object::primitive