/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"

#include "util/middleware/Helpers.hpp"

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::map<std::string, std::any> &args)
        : ABasicMaterial(args),
          _color(util::Helpers::toColor(args, "color", "FlatColor")) {
    }

    primitive::MaterialProperties FlatColor::evaluate(
        const primitive::SurfaceData &data,
        const maths::Vector &hitPoint) const {
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
