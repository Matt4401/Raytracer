/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** WorleyNoise
*/

#include "WorleyNoise.hpp"

#include <algorithm>
#include <cmath>

#include "../ColorHelpers.hpp"

namespace raytracer::object::material {
    WorleyNoise::WorleyNoise(const std::map<std::string, std::any>& args)
        : AWorleyBasedMaterial(args) {
    }

    primitive::MaterialProperties WorleyNoise::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        auto [F1, F2] = getWorley(hitPoint.x * _scale, hitPoint.y * _scale,
                                  hitPoint.z * _scale);
        double n = std::clamp(1.0 - F1, 0.0, 1.0);

        if (F2 - F1 < 0.02)
            n = 0.0;

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