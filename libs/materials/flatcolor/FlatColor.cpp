/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::vector<std::any> &args)
        : _color(util::ObjectMiddleware::validate<maths::Color>(
              args, 0, "FlatColor", EXPECTED_ARGS)),
          _emission(util::ObjectMiddleware::validate<maths::Vector>(
              args, 1, "FlatColor", EXPECTED_ARGS)),
          _refl(util::ObjectMiddleware::validate<raytracer::object::primitive::RefltT>(
              args, 2, "FlatColor", EXPECTED_ARGS)) {

        util::ObjectMiddleware::color(_color, "FlatColor");
    }

    void FlatColor::apply(raytracer::object::primitive::SurfaceData& data, const maths::Vector& hitPoint) const {
        data.color = _color;
        data.emission = _emission;
        data.reflType = _refl;
    }

} // namespace raytracer::object::material
