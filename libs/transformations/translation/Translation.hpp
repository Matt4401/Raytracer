/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Translation primitive decorator
*/

#pragma once

#include <any>

#include "math/Matrix4.hpp"
#include "object/AObject.hpp"
#include "object/primitive/APrimitiveDecorator.hpp"

namespace raytracer::object::primitive {
    class Translation : public APrimitiveDecorator, public object::AObject {
      public:
        explicit Translation(const std::map<std::string, std::any> &params);

        const std::string &name() const noexcept override;
        maths::Vector center() const noexcept override;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      private:
        maths::Matrix4 _matrix;
        maths::Matrix4 _inverse;
    };
}  // namespace raytracer::object::primitive
