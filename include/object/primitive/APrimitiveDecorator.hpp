/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APrimitiveDecorator
*/

#pragma once

#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::primitive {
    class APrimitiveDecorator : public IPrimitive {
      public:
        explicit APrimitiveDecorator(std::shared_ptr<IPrimitive> wrapped)
            : _wrapped(std::move(wrapped)) {}

        double hits(const maths::Ray &ray) override {
            return _wrapped->hits(ray);
        }

        BoundingBox boundingBox() override {
            return _wrapped->boundingBox();
        }

        SurfaceData surfaceData(const maths::Vector& hitPoint) const override {
            return _wrapped->surfaceData(hitPoint);
        }

        Type type() const override {
            return _wrapped->type();
        }

      protected:
        std::shared_ptr<IPrimitive> _wrapped;
    };
}