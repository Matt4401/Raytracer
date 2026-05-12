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

        double hits(const maths::Ray &ray) const override {
            return _wrapped->hits(ray);
        }

        bool hits(const maths::Ray &ray, HitRecord &record) const override {
            return _wrapped->hits(ray, record);
        }

        AABoundingBox boundingBox() override {
            return _wrapped->boundingBox();
        }

        SurfaceData surfaceData(const maths::Vector &hitPoint) const override {
            return _wrapped->surfaceData(hitPoint);
        }

        void setId(int id) override {
            _wrapped->setId(id);
        }

        int getId() const override {
            return _wrapped->getId();
        }

      protected:
        std::shared_ptr<IPrimitive> _wrapped;
    };
}  // namespace raytracer::object::primitive