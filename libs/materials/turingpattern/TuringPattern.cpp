/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TuringPattern
*/

#include "TuringPattern.hpp"

#include <algorithm>
#include <cmath>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {
    TuringPattern::TuringPattern(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
        _offset = util::ObjectMiddleware::optional<double>(
            args, "offset", 0.0, "TuringPattern");
        _threshold = util::ObjectMiddleware::optional<double>(
            args, "threshold", 0.05, "TuringPattern");
        util::Helpers::unsignedDouble(_threshold, "threshold", "TuringPattern");
        if (_threshold <= 0.0)
            _threshold = 0.05;
    }

    primitive::MaterialProperties TuringPattern::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale, hitPoint.z * _scale);
        double dist = std::abs(noise - _offset);

        double edgeSoftness = 0.03;
        double minEdge = _threshold - edgeSoftness;
        double maxEdge = _threshold + edgeSoftness;
        double n = 0.0;

        if (dist <= minEdge) {
            n = 1.0;
        } else if (dist >= maxEdge) {
            n = 0.0;
        } else {
            double t = (dist - minEdge) / (maxEdge - minEdge);
            n = 1.0 - (t * t * (3.0 - 2.0 * t));
        }

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