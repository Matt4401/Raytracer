/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Caustics
*/

#include "Caustics.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    Caustics::Caustics(const std::map<std::string, std::any>& args)
        : AWorleyBasedMaterial(args) {
    }

    primitive::MaterialProperties Caustics::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        auto [F1, F2] = getWorley(hitPoint.x * _scale, hitPoint.y * _scale,
                                  hitPoint.z * _scale);

        double edgeDist = F2 - F1;
        double invertedEdge = 1.0 - edgeDist;

        double n = std::pow(std::clamp(invertedEdge, 0.0, 1.0), 8.0);

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

        double glow = 2.5;
        maths::Vector localEmission =
            maths::Vector(_color2.r / 255.0, _color2.g / 255.0,
                          _color2.b / 255.0) *
            n * glow;

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