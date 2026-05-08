/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PerlinNoise
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class PerlinNoise : public ABasicMaterial {
      public:
        explicit PerlinNoise(const std::map<std::string, std::any>& args);
        ~PerlinNoise() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color1;
        maths::Color _color2;
        double _scale;
        int _octaves;
        double _persistence;
    };
}  // namespace raytracer::object::material
