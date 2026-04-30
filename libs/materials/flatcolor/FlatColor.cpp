/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::map<std::string, std::any> &args)
        : AMaterial(),
          _color(util::Helpers::toColor(args, "color", "FlatColor")),
          _emission(util::Helpers::toVector(args, "emission", "FlatColor")),
          _refl(util::ObjectMiddleware::validate<raytracer::object::primitive::RefltT>(
              args, "reflType", "FlatColor")) {
    }

    void FlatColor::apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const {
        data.color = _color;
        data.emission = _emission;
        data.reflType = _refl;
    }

} // namespace raytracer::object::material
