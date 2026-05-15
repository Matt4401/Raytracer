/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ATransformDecorator
*/

#pragma once

#include "math/Matrix4.hpp"
#include "object/AObject.hpp"
#include "object/primitive/APrimitiveDecorator.hpp"

namespace raytracer::object::primitive {
    class ATransformDecorator : public APrimitiveDecorator,
                                public object::AObject {
      public:
        ATransformDecorator(Type type)
            : APrimitiveDecorator(nullptr), AObject(type) {
        }

        const std::string &name() const noexcept override {
            return _wrapped->name();
        }
        maths::Vector center() const noexcept override {
            return _wrapped->center();
        }

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      protected:
        maths::Matrix4 _matrix;
        maths::Matrix4 _inverse;
        maths::Matrix4 _inverseTranspose;
    };
}  // namespace raytracer::object::primitive