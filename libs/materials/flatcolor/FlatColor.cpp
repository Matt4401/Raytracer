/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::map<std::string, std::any> &args)
        : _color(util::ObjectMiddleware::validate<maths::Color>(
              args, "color", "FlatColor")),
          _emission(util::ObjectMiddleware::validate<maths::Vector>(
              args, "emission", "FlatColor")),
          _refl(util::ObjectMiddleware::validate<raytracer::object::primitive::RefltT>(
              args, "reflType", "FlatColor")) {

        util::ObjectMiddleware::color(_color, "FlatColor");
    }

    void FlatColor::apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const {
        data.color = _color;
        data.emission = _emission;
        data.reflType = _refl;
    }

} // namespace raytracer::object::material
