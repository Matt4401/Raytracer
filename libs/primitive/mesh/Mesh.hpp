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
#include <string>

#include "MeshSurfaceHelper.hpp"
#include "ObjLoader.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Mesh : public APrimitive {
      public:
        Mesh(const std::map<std::string, std::any> &params);
        ~Mesh() override = default;
        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;

      protected:
      private:
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;
        BoundingBox triangleBoundingBox(int triangleIndex) const;
        void buildTrianglesFromFaces();

        BoundingBox _meshBoundingBox{};
        mutable std::optional<std::size_t> _lastHitTriangleIndex;

        std::unique_ptr<MeshSurfaceHelper> _surfaceHelper;
        std::unique_ptr<ObjLoader> _objLoader;
    };
}  // namespace raytracer::object::primitive
#endif /* !MESH_HPP_ */
