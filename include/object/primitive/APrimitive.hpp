/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** APrimitive
*/

#pragma once

#include <object/IObject.hpp>
#include <string>
#include <string_view>
#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/AObject.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::primitive {
    class APrimitive : public IPrimitive, public AObject {
      public:
        explicit APrimitive(const std::string_view name,
                            const maths::Vector &center,
                            const maths::Vector &emission,
                            const maths::Color &color,
                            const RefltT refl = RefltT::DIFF)
            : AObject(Type::PRIMITIVE),
              _name(name),
              _center(center),
              _emission(emission),
              _color(color),
              _refl(refl) {
        }
        ~APrimitive() override = default;

        double hits(const maths::Ray &ray) override = 0;
        BoundingBox boundingBox() override = 0;

        maths::Vector center() const noexcept;
        maths::Vector emission() const noexcept;
        maths::Color color() const noexcept;
        RefltT refl() const noexcept;
        const std::string &name() const noexcept;

      protected:
        std::string _name;
        maths::Vector _center;
        maths::Vector _emission;
        maths::Color _color;
        RefltT _refl;
    };
}  // namespace raytracer::object::primitive
