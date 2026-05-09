/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APerlinBasedMaterial
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class APerlinBasedMaterial : public ABasicMaterial {
      public:
        explicit APerlinBasedMaterial(
            const std::map<std::string, std::any>& args);
        ~APerlinBasedMaterial() override = default;

      protected:
        double calculateFBM(double x, double y, double z) const;

        maths::Color _color1;
        maths::Color _color2;
        double _scale;
        int _octaves;
        double _persistence;
        double _turbulence;
    };
}  // namespace raytracer::object::material