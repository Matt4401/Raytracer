/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** APrimitive
*/

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/AObject.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/IObject.hpp"
#include "object/material/IMaterial.hpp"

namespace raytracer::object::primitive {
    class APrimitive : public IPrimitive, public AObject {
      public:
        explicit APrimitive(const std::string_view name,
                            const maths::Vector &center,
                            std::shared_ptr<raytracer::object::material::IMaterial> material = nullptr)
            : AObject(Type::PRIMITIVE),
              _material(std::move(material)),
              _name(name),
              _center(center) {

        }
        ~APrimitive() override = default;

        double hits(const maths::Ray &ray) override = 0;
        BoundingBox boundingBox() override = 0;
        SurfaceData surfaceData(const maths::Vector& hitPoint) const override = 0;

        maths::Vector center() const noexcept;
        const std::string &name() const noexcept;

      protected:
        std::shared_ptr<raytracer::object::material::IMaterial> _material;
        std::string _name;
        maths::Vector _center;
    };
}  // namespace raytracer::object::primitive
