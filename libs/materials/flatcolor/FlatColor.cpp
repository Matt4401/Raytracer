/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"

#include <algorithm>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::map<std::string, std::any>& args)
        : AMaterial(),
          _color(util::Helpers::toColor(args, "color", "FlatColor")),
          _emission(0, 0, 0),
          _refl(util::ObjectMiddleware::validate<
                raytracer::object::primitive::RefltT>(args, "reflType",
                                                      "FlatColor")),
          _reflectivity(0.0),
          _transparency(0.0),
          _ior(1.0),
          _roughness(0.0),
          _metalness(0.0) {
        _emission = util::Helpers::optionalVector(args, "emission", _emission,
                                                   "FlatColor");
        _reflectivity = util::ObjectMiddleware::optional<double>(
            args, "reflectivity", 0.0, "FlatColor");
        util::Helpers::unsignedDouble(_reflectivity, "reflectivity",
                                      "FlatColor");
        _reflectivity = std::min(_reflectivity, 1.0);

        _transparency = util::ObjectMiddleware::optional<double>(
            args, "transparency", 0.0, "FlatColor");
        util::Helpers::unsignedDouble(_transparency, "transparency",
                                      "FlatColor");
        _transparency = std::min(_transparency, 1.0);

        _ior = util::ObjectMiddleware::optional<double>(args, "ior", 1.0,
                                                        "FlatColor");
        util::Helpers::unsignedDouble(_ior, "ior", "FlatColor");
        if (_ior <= 0.0)
            _ior = 1.0;

        _roughness = util::ObjectMiddleware::optional<double>(
            args, "roughness", 0.0, "FlatColor");
        util::Helpers::unsignedDouble(_roughness, "roughness", "FlatColor");
        _roughness = std::min(_roughness, 1.0);

        _metalness = util::ObjectMiddleware::optional<double>(
            args, "metalness", 0.0, "FlatColor");
        util::Helpers::unsignedDouble(_metalness, "metalness", "FlatColor");
        _metalness = std::min(_metalness, 1.0);
    }

    primitive::MaterialProperties FlatColor::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        return {.color = _color,
                .emission = _emission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness};
    }

}  // namespace raytracer::object::material
