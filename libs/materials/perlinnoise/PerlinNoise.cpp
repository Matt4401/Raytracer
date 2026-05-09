/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    PerlinNoise::PerlinNoise(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties PerlinNoise::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        const double x = hitPoint.x * _scale;
        const double y = hitPoint.y * _scale;
        const double z = hitPoint.z * _scale;

        double amplitude = 1.0;
        double frequency = 1.0;
        double noise = 0.0;
        double maxValue = 0.0;
        for (int i = 0; i < _octaves; ++i) {
            noise +=
                perlin(x * frequency, y * frequency, z * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= _persistence;
            frequency *= 2.0;
        }
        noise /= maxValue;

        const double n = (noise + 1.0) * 0.5;

        maths::Color finalColor = maths::Color(
            static_cast<unsigned char>(
                std::clamp(static_cast<int>(
                               std::round(_color1.r * (1 - n) + _color2.r * n)),
                           0, 255)),
            static_cast<unsigned char>(
                std::clamp(static_cast<int>(
                               std::round(_color1.g * (1 - n) + _color2.g * n)),
                           0, 255)),
            static_cast<unsigned char>(
                std::clamp(static_cast<int>(
                               std::round(_color1.b * (1 - n) + _color2.b * n)),
                           0, 255)));

        return {.color = finalColor,
                .emission = _emission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness};
    }

}  // namespace raytracer::object::material