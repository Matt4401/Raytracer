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
    std::optional<MeshSurfaceHelper::TriangleIntersection>
    MeshSurfaceHelper::findClosestTriangle(const maths::Ray &ray) const {
        double closestT = std::numeric_limits<double>::infinity();
        int closestIdx = -1;

        for (int i = 0; i < static_cast<int>(_triangles.size()); ++i) {
            const auto &tri = _triangles[i];
            const maths::Vector &v0 = _vertices.get()[tri.v1];
            const maths::Vector &v1 = _vertices.get()[tri.v2];
            const maths::Vector &v2 = _vertices.get()[tri.v3];

            double t;
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
            triangleIdx >= static_cast<int>(_triangles.size())) {
            return maths::Vector(0, 1, 0);
        }

        const auto &tri = _triangles[triangleIdx];

        // Use interpolated normal if available
        if (!normals.empty() && normals.size() == _vertices.get().size()) {
            const maths::Vector &n0 = normals[tri.v1];
            const maths::Vector &n1 = normals[tri.v2];
            const maths::Vector &n2 = normals[tri.v3];
            return ((n0 + n1 + n2) / 3.0).normalized();
        }

        // Compute normal from triangle geometry
        const maths::Vector &v0 = _vertices.get()[tri.v1];
        const maths::Vector &v1 = _vertices.get()[tri.v2];
        const maths::Vector &v2 = _vertices.get()[tri.v3];
        maths::Vector normal = (v1 - v0).cross(v2 - v0);

        if (normal.magnitude() < K_RAY_EPSILON) {
            return maths::Vector(0, 1, 0);
        }
        return normal.normalized();
    }

    maths::Vector MeshSurfaceHelper::computeUV(const maths::Vector &hitPoint,
                                               const maths::Vector &center) {
        double u = hitPoint.x - center.x;
        double v = hitPoint.y - center.y;
        return maths::Vector(u, v, 0);
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

        if (std::fabs(a) < K_RAY_EPSILON) {
            return false;
        }

        const double f = 1.0 / a;
        const maths::Vector s = ray.origin - v0;
        double u = f * s.dot(h);
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        const maths::Vector q = s.cross(edge1);
        double v = f * ray.direction.dot(q);
        if (v < 0.0 || u + v > 1.0) {
            return false;
        }

        double t = f * edge2.dot(q);
        if (t > K_RAY_EPSILON) {
            outT = t;
            return true;
        }
        return false;
    }

}  // namespace raytracer::object::primitive
