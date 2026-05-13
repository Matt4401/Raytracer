/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Face
*/

#include "Face.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::primitive {
    Face::Face(std::size_t faceIndex,
               std::shared_ptr<const MeshBuffers> buffers,
               const IndexedTriangle &triangle,
               std::shared_ptr<raytracer::object::material::IMaterial> material)
        : APrimitive("Face", triangleCenter(buffers, triangle),
                     std::move(material)),
          _index(faceIndex),
          _buffers(std::move(buffers)),
          _triangle(triangle) {
    }

    maths::Vector Face::triangleCenter(
        const std::shared_ptr<const MeshBuffers> &buffers,
        const IndexedTriangle &triangle) {
        if (!buffers || triangle.v0 < 0 || triangle.v1 < 0 || triangle.v2 < 0 ||
            triangle.v0 >= static_cast<int>(buffers->vertices.size()) ||
            triangle.v1 >= static_cast<int>(buffers->vertices.size()) ||
            triangle.v2 >= static_cast<int>(buffers->vertices.size())) {
            return maths::Vector(0, 0, 0);
        }
        return (buffers->vertices[static_cast<std::size_t>(triangle.v0)] +
                buffers->vertices[static_cast<std::size_t>(triangle.v1)] +
                buffers->vertices[static_cast<std::size_t>(triangle.v2)]) /
               3.0;
    }

    maths::Vector Face::vertexOrDefault(const int index) const {
        if (!_buffers || index < 0 ||
            index >= static_cast<int>(_buffers->vertices.size())) {
            return maths::Vector(0, 0, 0);
        }
        return _buffers->vertices[static_cast<std::size_t>(index)];
    }

    maths::Vector Face::texCoordOrDefault(const int index) const {
        if (!_buffers || index < 0 ||
            index >= static_cast<int>(_buffers->texCoords.size())) {
            return maths::Vector(0, 0, 0);
        }
        return _buffers->texCoords[static_cast<std::size_t>(index)];
    }

    maths::Vector Face::normalOrDefault(const int index) const {
        if (!_buffers || index < 0 ||
            index >= static_cast<int>(_buffers->normals.size())) {
            return maths::Vector(0, 0, 0);
        }
        return _buffers->normals[static_cast<std::size_t>(index)];
    }

    bool Face::hits(const maths::Ray &ray, HitRecord &record) const {
        const maths::Vector v0 = vertexOrDefault(_triangle.v0);
        const maths::Vector v1 = vertexOrDefault(_triangle.v1);
        const maths::Vector v2 = vertexOrDefault(_triangle.v2);
        const maths::Vector edge1 = v1 - v0;
        const maths::Vector edge2 = v2 - v0;
        const maths::Vector h = ray.direction.cross(edge2);
        const double a = edge1.dot(h);

        if (std::abs(a) < K_RAY_EPSILON) {
            return false;
        }

        const double f = 1.0 / a;
        const maths::Vector s = ray.origin - v0;
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
        record.objectId = id();
        record.hitPoint = ray.origin + ray.direction * t;
        return true;
    }

    IPrimitive::AABoundingBox Face::boundingBox() {
        const maths::Vector v0 = vertexOrDefault(_triangle.v0);
        const maths::Vector v1 = vertexOrDefault(_triangle.v1);
        const maths::Vector v2 = vertexOrDefault(_triangle.v2);
        const double minX = std::min({v0.x, v1.x, v2.x});
        const double minY = std::min({v0.y, v1.y, v2.y});
        const double minZ = std::min({v0.z, v1.z, v2.z});
        const double maxX = std::max({v0.x, v1.x, v2.x});
        const double maxY = std::max({v0.y, v1.y, v2.y});
        const double maxZ = std::max({v0.z, v1.z, v2.z});

        return {minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ};
    }

    SurfaceData Face::surfaceData(const HitRecord &record) const {
        const maths::Vector &hitPoint = record.hitPoint;
        const maths::Vector v0 = vertexOrDefault(_triangle.v0);
        const maths::Vector v1 = vertexOrDefault(_triangle.v1);
        const maths::Vector v2 = vertexOrDefault(_triangle.v2);
        const maths::Vector uv0 = texCoordOrDefault(_triangle.vt0);
        const maths::Vector uv1 = texCoordOrDefault(_triangle.vt1);
        const maths::Vector uv2 = texCoordOrDefault(_triangle.vt2);
        maths::Vector n0 = normalOrDefault(_triangle.vn0);
        maths::Vector n1 = normalOrDefault(_triangle.vn1);
        maths::Vector n2 = normalOrDefault(_triangle.vn2);

        const maths::Vector edge1 = v1 - v0;
        const maths::Vector edge2 = v2 - v0;

        if (n0.lenght() < 1e-6 && n1.lenght() < 1e-6 && n2.lenght() < 1e-6) {
            const maths::Vector faceNormal = edge1.cross(edge2).normalized();
            n0 = faceNormal;
            n1 = faceNormal;
            n2 = faceNormal;
        }

        const maths::Vector localHit = hitPoint - v0;
        const double d00 = edge1.dot(edge1);
        const double d01 = edge1.dot(edge2);
        const double d11 = edge2.dot(edge2);
        const double d20 = localHit.dot(edge1);
        const double d21 = localHit.dot(edge2);
        const double denom = d00 * d11 - d01 * d01;

        double u = 0.0;
        double v = 0.0;
        double w = 1.0;

        if (std::abs(denom) > K_RAY_EPSILON) {
            u = (d20 * d11 - d21 * d01) / denom;
            v = (d21 * d00 - d20 * d01) / denom;
            w = 1.0 - u - v;
        }

        maths::Vector uv = uv0 * w + uv1 * u + uv2 * v;
        maths::Vector normal = (n0 * w + n1 * u + n2 * v).normalized();

        std::string materialName;
        if (_triangle.materialIndex >= 0 && _buffers &&
            _triangle.materialIndex <
                static_cast<int>(_buffers->materialNames.size())) {
            materialName = _buffers->materialNames[static_cast<std::size_t>(
                _triangle.materialIndex)];
        }

        SurfaceData data{.normal = normal,
                         .uv = uv,
                         .materialName = materialName,
                         .material = {}};
        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }
        return data;
    }

    std::size_t Face::index() const noexcept {
        return _index;
    }
}  // namespace raytracer::object::primitive