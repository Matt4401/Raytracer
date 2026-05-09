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
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class FlatColor : public ABasicMaterial {
      public:
        explicit FlatColor(const std::map<std::string, std::any>& args);
        ~FlatColor() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color;
    };
}  // namespace raytracer::object::material
