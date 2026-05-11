/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** CrackedDirt
*/

#include "CrackedDirt.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    CrackedDirt::CrackedDirt(const std::map<std::string, std::any>& args)
        : AWorleyBasedMaterial(args) {
    }

    primitive::MaterialProperties CrackedDirt::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        auto [F1, F2] = getWorley(hitPoint.x * _scale, hitPoint.y * _scale,
                                  hitPoint.z * _scale);

        double edgeDistance = F2 - F1;
        double n = 1.0;
        if (edgeDistance < 0.1) {
            n = edgeDistance * 10.0;
        }

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