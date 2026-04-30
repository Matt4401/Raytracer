/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#pragma once

#include "object/material/AMaterial.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/ReflTypes.hpp"

#include <any>
#include <map>
#include <string>

namespace raytracer::object::material {
    class FlatColor : public AMaterial {
      public:
        explicit FlatColor(const std::map<std::string, std::any> &args);
        ~FlatColor() override = default;

        void apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color;
        maths::Vector _emission;
        raytracer::object::primitive::RefltT _refl;
    };
}
