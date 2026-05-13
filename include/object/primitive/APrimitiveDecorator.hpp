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

        void setLimitBox(const AABoundingBox &box) override {
            _wrapped->setLimitBox(box);
        }

        bool isInfinite() const override {
            return _wrapped->isInfinite();
        }

        AABoundingBox boundingBox() override {
            return _wrapped->boundingBox();
        }

        SurfaceData surfaceData(const HitRecord &record) const override {
            return _wrapped->surfaceData(record);
        }

        void setId(int id) override {
            _wrapped->setId(id);
        }

        int id() const override {
            return _wrapped->id();
        }

      protected:
        std::shared_ptr<IPrimitive> _wrapped;
    };
}  // namespace raytracer::object::primitive