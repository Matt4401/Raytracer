/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MeshSurfaceHelper
*/

#ifndef MESHSURFACEHELPER_HPP_
#define MESHSURFACEHELPER_HPP_

#include <functional>
#include <optional>
#include <vector>

#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    class MeshSurfaceHelper {
      public:
        struct Triangle {
            int v1;
            int v2;
            int v3;
        };

        struct TriangleIntersection {
            int triangleIndex;
            double distance;
            maths::Vector hitPoint;
        };

        explicit MeshSurfaceHelper(const std::vector<maths::Vector> &vertices)
            : _vertices(std::cref(vertices)) {
        }

        ~MeshSurfaceHelper() = default;

        void addTriangle(int v1, int v2, int v3) {
            _triangles.emplace_back(v1, v2, v3);
        }

        void addTriangle(const Triangle &tri) {
            _triangles.push_back(tri);
        }

        const std::vector<Triangle> &triangles() const {
            return _triangles;
        }

        int triangleCount() const {
            return _triangles.size();
        }

        /**
         * @brief Find the closest triangle intersection with a ray
         */
        std::optional<TriangleIntersection> findClosestTriangle(
            const maths::Ray &ray) const;

        /**
         * @brief Compute surface normal at intersection point
         */
        maths::Vector computeNormal(
            int triangleIdx, const maths::Vector &hitPoint,
            const std::vector<maths::Vector> &normals) const;

        /**
         * @brief Compute UV coordinates for surface
         */
        static maths::Vector computeUV(const maths::Vector &hitPoint,
                                       const maths::Vector &center);

      private:
        static const double K_RAY_EPSILON;

        bool triangleIntersection(const maths::Ray &ray,
                                  const maths::Vector &v0,
                                  const maths::Vector &v1,
                                  const maths::Vector &v2, double &outT) const;

        std::reference_wrapper<const std::vector<maths::Vector>> _vertices;
        std::vector<Triangle> _triangles;
    };

}  // namespace raytracer::object::primitive

#endif /* !MESHSURFACEHELPER_HPP_ */
