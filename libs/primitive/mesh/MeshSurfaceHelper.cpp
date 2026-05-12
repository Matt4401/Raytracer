/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MeshSurfaceHelper
*/

#include "MeshSurfaceHelper.hpp"

#include <cmath>
#include <limits>

namespace raytracer::object::primitive {

    bool MeshSurfaceHelper::pointInTriangle(const maths::Vector &point,
                                            const maths::Vector &v0,
                                            const maths::Vector &v1,
                                            const maths::Vector &v2) const {
        const maths::Vector edge0 = v1 - v0;
        const maths::Vector edge1 = v2 - v0;
        const maths::Vector toPoint = point - v0;

        const double dot00 = edge0.dot(edge0);
        const double dot01 = edge0.dot(edge1);
        const double dot11 = edge1.dot(edge1);
        const double dot20 = toPoint.dot(edge0);
        const double dot21 = toPoint.dot(edge1);
        const double denom = dot00 * dot11 - dot01 * dot01;

        if (std::fabs(denom) <= 1e-10) {
            return false;
        }

        const double u = (dot11 * dot20 - dot01 * dot21) / denom;
        const double v = (dot00 * dot21 - dot01 * dot20) / denom;
        return u >= -1e-6 && v >= -1e-6 && (u + v) <= 1.0 + 1e-6;
    }

    std::optional<MeshSurfaceHelper::TriangleIntersection>
    MeshSurfaceHelper::findClosestTriangle(const maths::Ray &ray) const {
        double closestT = std::numeric_limits<double>::infinity();
        int closestIdx = -1;

        for (int i = 0; i < static_cast<int>(_faces.get().size()); ++i) {
            const auto &face = _faces.get()[i];
            const maths::Vector &v0 = _vertices.get()[face.fv1.v];
            const maths::Vector &v1 = _vertices.get()[face.fv2.v];
            const maths::Vector &v2 = _vertices.get()[face.fv3.v];

            double t = 0.0;
            if (triangleIntersection(ray, v0, v1, v2, t)) {
                if (t > K_RAY_EPSILON && t < closestT) {
                    closestT = t;
                    closestIdx = i;
                }
            }
        }

        if (closestIdx >= 0) {
            return TriangleIntersection{
                .triangleIndex = closestIdx,
                .distance = closestT,
                .hitPoint = ray.origin + ray.direction * closestT};
        }
        return std::nullopt;
    }

    maths::Vector MeshSurfaceHelper::computeNormal(
        int triangleIdx, const maths::Vector &hitPoint,
        const std::vector<maths::Vector> &normals) const {
        if (triangleIdx < 0 ||
            triangleIdx >= static_cast<int>(_faces.get().size()))
            return maths::Vector(0, 1, 0);

        const auto &face = _faces.get()[triangleIdx];

        if (!normals.empty() && face.fv1.vn >= 0 && face.fv2.vn >= 0 &&
            face.fv3.vn >= 0 &&
            face.fv1.vn < static_cast<int>(normals.size()) &&
            face.fv2.vn < static_cast<int>(normals.size()) &&
            face.fv3.vn < static_cast<int>(normals.size())) {
            const maths::Vector &n0 = normals[face.fv1.vn];
            const maths::Vector &n1 = normals[face.fv2.vn];
            const maths::Vector &n2 = normals[face.fv3.vn];
            return ((n0 + n1 + n2) / 3.0).normalized();
        }

        const maths::Vector &v0 = _vertices.get()[face.fv1.v];
        const maths::Vector &v1 = _vertices.get()[face.fv2.v];
        const maths::Vector &v2 = _vertices.get()[face.fv3.v];
        maths::Vector normal = (v1 - v0).cross(v2 - v0);

        if (normal.magnitude() < K_RAY_EPSILON)
            return maths::Vector(0, 1, 0);
        return normal.normalized();
    }

    maths::Vector MeshSurfaceHelper::computeUV(
        int triangleIdx, const maths::Vector &hitPoint,
        const std::vector<maths::Vector> &textureCoords) const {
        if (triangleIdx >= 0 &&
            triangleIdx < static_cast<int>(_faces.get().size())) {
            const auto &face = _faces.get()[triangleIdx];

            if (!textureCoords.empty() && face.fv1.vt >= 0 &&
                face.fv2.vt >= 0 && face.fv3.vt >= 0 &&
                face.fv1.vt < static_cast<int>(textureCoords.size()) &&
                face.fv2.vt < static_cast<int>(textureCoords.size()) &&
                face.fv3.vt < static_cast<int>(textureCoords.size())) {
                const maths::Vector &v0 = _vertices.get()[face.fv1.v];
                const maths::Vector &v1 = _vertices.get()[face.fv2.v];
                const maths::Vector &v2 = _vertices.get()[face.fv3.v];

                const maths::Vector e1 = v1 - v0;
                const maths::Vector e2 = v2 - v0;
                const maths::Vector ep = hitPoint - v0;

                const double d11 = e1.dot(e1);
                const double d12 = e1.dot(e2);
                const double d22 = e2.dot(e2);
                const double d1p = e1.dot(ep);
                const double d2p = e2.dot(ep);

                const double denom = d11 * d22 - d12 * d12;
                double bv = 0.0, bw = 0.0;
                if (std::fabs(denom) > K_RAY_EPSILON) {
                    bv = (d22 * d1p - d12 * d2p) / denom;
                    bw = (d11 * d2p - d12 * d1p) / denom;
                }
                const double bu = 1.0 - bv - bw;

                const maths::Vector &uv0 = textureCoords[face.fv1.vt];
                const maths::Vector &uv1 = textureCoords[face.fv2.vt];
                const maths::Vector &uv2 = textureCoords[face.fv3.vt];

                return uv0 * bu + uv1 * bv + uv2 * bw;
            }
        }
        return maths::Vector(hitPoint.x, hitPoint.y,
                             0);  // TODO : fallback UV mapping
    }

    maths::Vector MeshSurfaceHelper::computeCentroid(
        const ObjLoader::Face &face) const {
        const auto &v0 = _vertices.get()[face.fv1.v];
        const auto &v1 = _vertices.get()[face.fv2.v];
        const auto &v2 = _vertices.get()[face.fv3.v];
        return (v0 + v1 + v2) / 3.0;
    }

    bool MeshSurfaceHelper::triangleIntersection(const maths::Ray &ray,
                                                 const maths::Vector &v0,
                                                 const maths::Vector &v1,
                                                 const maths::Vector &v2,
                                                 double &outT) const {
        const maths::Vector edge1 = v1 - v0;
        const maths::Vector edge2 = v2 - v0;
        const maths::Vector h = ray.direction.cross(edge2);
        const double a = edge1.dot(h);

        if (std::fabs(a) < K_RAY_EPSILON)
            return false;

        const double f = 1.0 / a;
        const maths::Vector s = ray.origin - v0;
        const double u = f * s.dot(h);
        if (u < 0.0 || u > 1.0)
            return false;

        const maths::Vector q = s.cross(edge1);
        const double v = f * ray.direction.dot(q);
        if (v < 0.0 || u + v > 1.0)
            return false;

        const double t = f * edge2.dot(q);
        if (t > K_RAY_EPSILON) {
            outT = t;
            return true;
        }
        return false;
    }

}  // namespace raytracer::object::primitive