/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#ifndef MESH_HPP_
#define MESH_HPP_

#include <any>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "MeshSurfaceHelper.hpp"
#include "ObjLoader.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Mesh : public APrimitive {
      public:
        Mesh(const std::map<std::string, std::any> &params);
        ~Mesh() override = default;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;
        AABoundingBox boundingBox() override;
      protected:
      private:
        
        void buildTrianglesFromFaces();

        std::unique_ptr<MeshSurfaceHelper> _surfaceHelper;
        std::unique_ptr<ObjLoader> _objLoader;

        mutable std::optional<std::size_t> _lastHitTriangleIndex;
        mutable maths::AABoundingBox _meshBoundingBox;

        // helper to get the bounding box of a specific triangle
        IPrimitive::AABoundingBox triangleBoundingBox(int triangleIndex) const;
    };
}  // namespace raytracer::object::primitive
#endif /* !MESH_HPP_ */
