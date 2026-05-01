/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/AMaterial.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::material {
    class FlatColor : public AMaterial {
      public:
        explicit FlatColor(const std::map<std::string, std::any>& args);
        ~FlatColor() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color;
        maths::Vector _emission;
        primitive::RefltT _refl;
        double _reflectivity;
        double _transparency;
        double _ior;
        double _roughness;
        double _metalness;
    };
}  // namespace raytracer::object::material
