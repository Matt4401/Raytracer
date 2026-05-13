/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Face
*/

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Face : public APrimitive {
      public:
        struct IndexedTriangle {
            int v0 = -1;
            int v1 = -1;
            int v2 = -1;
            int vt0 = -1;
            int vt1 = -1;
            int vt2 = -1;
            int vn0 = -1;
            int vn1 = -1;
            int vn2 = -1;
            int materialIndex = -1;
        };

        struct MeshBuffers {
            std::vector<maths::Vector> vertices;
            std::vector<maths::Vector> texCoords;
            std::vector<maths::Vector> normals;
            std::vector<std::string> materialNames;
        };

        Face(std::size_t faceIndex, std::shared_ptr<const MeshBuffers> buffers,
             const IndexedTriangle &triangle,
             std::shared_ptr<raytracer::object::material::IMaterial> material =
                 nullptr);
        ~Face() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

        std::size_t index() const noexcept;

      private:
        static maths::Vector triangleCenter(
            const std::shared_ptr<const MeshBuffers> &buffers,
            const IndexedTriangle &triangle);
        maths::Vector vertexOrDefault(int index) const;
        maths::Vector texCoordOrDefault(int index) const;
        maths::Vector normalOrDefault(int index) const;

        std::size_t _index;
        std::shared_ptr<const MeshBuffers> _buffers;
        IndexedTriangle _triangle;
    };
}  // namespace raytracer::object::primitive