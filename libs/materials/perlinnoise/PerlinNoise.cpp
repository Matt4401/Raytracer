/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"

#include <cmath>

#include "../ColorHelpers.hpp"

namespace raytracer::object::material {
    PerlinNoise::PerlinNoise(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties PerlinNoise::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale,
                                    hitPoint.z * _scale);

        const double n = (noise + 1.0) * 0.5;

        const maths::Color finalColor =
            raytracer::materials::helpers::interpolateColors(_color1, _color2,
                                                             n);

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