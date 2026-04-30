/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#pragma once

#include "object/material/IMaterial.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/ReflTypes.hpp"

#include <any>
#include <vector>

namespace raytracer::object::material {
    class FlatColor : public IMaterial {
      public:
        explicit FlatColor(const std::vector<std::any> &args);
        ~FlatColor() override = default;

        void apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color;
        maths::Vector _emission;
        raytracer::object::primitive::RefltT _refl;

        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}
