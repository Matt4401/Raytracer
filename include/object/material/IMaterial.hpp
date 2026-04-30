#pragma once

#include "object/IObject.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::material {
    class IMaterial : public IObject {
      public:
        ~IMaterial() override = default;

        virtual void apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const = 0;

        Type type() const override {
            return Type::MATERIAL;
        }
    };
}
