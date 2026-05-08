/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Magma
*/

#include "Magma.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    Magma::Magma(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties Magma::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale, hitPoint.z * _scale);
        double ridge = 1.0 - std::abs(noise);
        double n = std::pow(ridge, _turbulence);
        n = std::clamp(n, 0.0, 1.0);

        maths::Color finalColor = maths::Color(
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.r * (1 - n) + _color2.r * n)), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.g * (1 - n) + _color2.g * n)), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.b * (1 - n) + _color2.b * n)), 0, 255))
        );

        double glowIntensity = 2.0;
        maths::Vector localEmission = maths::Vector(
            (_color2.r / 255.0) * n * glowIntensity,
            (_color2.g / 255.0) * n * glowIntensity,
            (_color2.b / 255.0) * n * glowIntensity
        );

        return {.color = finalColor,
                .emission = localEmission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness};
    }

}  // namespace raytracer::object::material