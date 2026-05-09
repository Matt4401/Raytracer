/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Wood
*/

#include "Wood.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    Wood::Wood(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties Wood::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        const double nx = hitPoint.x * _scale;
        const double ny = hitPoint.y * _scale * 0.1;
        const double nz = hitPoint.z * _scale;

        const double noise = calculateFBM(nx, ny, nz);
        const double radius =
            std::sqrt(hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z);
        const double woodValue =
            std::sin(radius * _scale * 3.0 + noise * _turbulence);
        double n = (woodValue + 1.0) * 0.5;
        n = std::pow(n, 0.7);

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