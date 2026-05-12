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

#include "ObjLoader.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    class MeshSurfaceHelper {
      public:
        struct TriangleIntersection {
            int triangleIndex;
            double distance;
            maths::Vector hitPoint;
        };

        explicit MeshSurfaceHelper(const std::vector<maths::Vector> &vertices,
                                   const std::vector<ObjLoader::Face> &faces)
            : _vertices(std::cref(vertices)), _faces(std::cref(faces)) {
        }

        ~MeshSurfaceHelper() = default;

        const std::vector<ObjLoader::Face> &triangles() const {
            return _faces.get();
        }

        int triangleCount() const {
            return static_cast<int>(_faces.get().size());
        }

        std::optional<TriangleIntersection> findClosestTriangle(
            const maths::Ray &ray) const;

        bool pointInTriangle(const maths::Vector &point,
                             const maths::Vector &v0, const maths::Vector &v1,
                             const maths::Vector &v2) const;

        maths::Vector computeNormal(
            int triangleIdx, const maths::Vector &hitPoint,
            const std::vector<maths::Vector> &normals) const;

        maths::Vector computeUV(
            int triangleIdx, const maths::Vector &hitPoint,
            const std::vector<maths::Vector> &textureCoords) const;

        maths::Vector computeCentroid(const ObjLoader::Face &face) const;

      private:
        static constexpr double K_RAY_EPSILON = 1e-4;

        bool triangleIntersection(const maths::Ray &ray,
                                  const maths::Vector &v0,
                                  const maths::Vector &v1,
                                  const maths::Vector &v2, double &outT) const;

        std::reference_wrapper<const std::vector<maths::Vector>> _vertices;
        std::reference_wrapper<const std::vector<ObjLoader::Face>> _faces;
    };

}  // namespace raytracer::object::primitive

#endif /* !MESHSURFACEHELPER_HPP_ */