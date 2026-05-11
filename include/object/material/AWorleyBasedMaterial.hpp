/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AWorleyBasedMaterial
*/

#pragma once

#include <any>
#include <map>
#include <string>
#include <utility>

#include "math/Color.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class AWorleyBasedMaterial : public ABasicMaterial {
      public:
        explicit AWorleyBasedMaterial(
            const std::map<std::string, std::any>& args);
        ~AWorleyBasedMaterial() override = default;

      protected:
        maths::Color _color1;
        maths::Color _color2;
        double _scale;

        std::pair<double, double> getWorley(double x, double y, double z) const;
    };
}  // namespace raytracer::object::material