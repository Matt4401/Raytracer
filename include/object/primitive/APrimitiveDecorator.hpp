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

                bool hits(const maths::Ray &ray,
                                    HitRecord &rec) const override {
                        return _wrapped->hits(ray, rec);
        }

        AABoundingBox boundingBox() override {
            return _wrapped->boundingBox();
        }

        SurfaceData surfaceData(const maths::Vector &hitPoint) const override {
            return _wrapped->surfaceData(hitPoint);
        }

      protected:
        std::shared_ptr<IPrimitive> _wrapped;
    };
}  // namespace raytracer::object::primitive