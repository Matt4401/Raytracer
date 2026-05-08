/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Marble
*/

#include "Marble.hpp"
#include <algorithm>
#include <cmath>

namespace raytracer::object::material {
    Marble::Marble(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties Marble::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {

        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale, hitPoint.z * _scale);

        double basePattern = (hitPoint.x + hitPoint.z) * _scale;
        double marbleValue = std::sin(basePattern + noise * _turbulence);
        double n = (marbleValue + 1.0) * 0.5;

        maths::Color finalColor = maths::Color(
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.r * (1 - n) + _color2.r * n)), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.g * (1 - n) + _color2.g * n)), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(std::round(_color1.b * (1 - n) + _color2.b * n)), 0, 255))
        );

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