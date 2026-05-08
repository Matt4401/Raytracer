/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APrimitiveDecorator
*/

#pragma once

#include <memory>

#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::primitive {
    class APrimitiveDecorator : public IPrimitive {
      public:
        explicit APrimitiveDecorator(std::shared_ptr<IPrimitive> wrapped)
            : _wrapped(std::move(wrapped)) {
        }

        std::optional<HitContext> hits(const maths::Ray &ray, bool computeSurfaceData = true) override {
            return _wrapped->hits(ray, computeSurfaceData);
        }

        BoundingBox boundingBox() override {
            return _wrapped->boundingBox();
        }

      protected:
        std::shared_ptr<IPrimitive> _wrapped;
    };
}  // namespace raytracer::object::primitive