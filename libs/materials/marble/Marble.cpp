/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Marble
*/

#include "Marble.hpp"

#include <cmath>

#include "../ColorHelpers.hpp"

namespace raytracer::object::material {
    Marble::Marble(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties Marble::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale,
                                    hitPoint.z * _scale);

        double basePattern = (hitPoint.x + hitPoint.z) * _scale;
        double marbleValue = std::sin(basePattern + noise * _turbulence);
        double n = (marbleValue + 1.0) * 0.5;

        maths::Color finalColor =
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