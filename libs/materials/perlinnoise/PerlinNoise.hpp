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

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class PerlinNoise : public APerlinBasedMaterial {
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

        double fade(double t) const;
        double grad(int hash, double x, double y, double z) const;
        double lerp(double a, double b, double t) const;
        double perlin(double x, double y, double z) const;
    };
}  // namespace raytracer::object::material
