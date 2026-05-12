/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Face
*/

#pragma once

#include <any>
#include <cstddef>
#include <memory>
#include <string>

#include "ObjLoader.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Face : public APrimitive {
      public:
        Face(std::size_t faceIndex, const ObjLoader::Face &face,
             const maths::Vector &v0, const maths::Vector &v1,
             const maths::Vector &v2,
             std::shared_ptr<raytracer::object::material::IMaterial> material =
                 nullptr,
             const std::string &materialName = "");

        ~Face() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

        const ObjLoader::Face &geometry() const noexcept;
        std::size_t index() const noexcept;

      private:
        ObjLoader::Face _face;
        std::size_t _index;
        std::string _materialName;
        maths::Vector _v0;
        maths::Vector _v1;
        maths::Vector _v2;
    };
}  // namespace raytracer::object::primitive