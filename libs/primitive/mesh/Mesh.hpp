/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Face.hpp"
#include "ObjLoader.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Mesh : public APrimitive {
      public:
        Mesh(const std::map<std::string, std::any> &params);
        ~Mesh() override = default;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        SurfaceData surfaceData(const HitRecord &record) const override;
        AABoundingBox boundingBox() override;

      protected:
      private:
        std::shared_ptr<IPrimitive> _bvhRoot;
        std::vector<std::shared_ptr<IPrimitive>> _faces;

        mutable maths::AABoundingBox _meshBoundingBox;
    };
}  // namespace raytracer::object::primitive
