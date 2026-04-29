/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** FlatColor
*/

#include "FlatColor.hpp"
#include "util/ObjectMiddleware.hpp"

#include <memory>

namespace raytracer::object::material {

    FlatColor::FlatColor(const std::vector<std::any> &args)
        : raytracer::object::primitive::APrimitiveDecorator(
              util::ObjectMiddleware::validate<std::shared_ptr<raytracer::object::primitive::IPrimitive>>(
                  args, 0, "FlatColor", EXPECTED_ARGS)),
          _color(util::ObjectMiddleware::validate<maths::Color>(
              args, 1, "FlatColor", EXPECTED_ARGS)),
          _emission(util::ObjectMiddleware::validate<maths::Vector>(
              args, 2, "FlatColor", EXPECTED_ARGS)),
          _refl(util::ObjectMiddleware::validate<raytracer::object::primitive::RefltT>(
              args, 3, "FlatColor", EXPECTED_ARGS)) {

        util::ObjectMiddleware::color(_color, "FlatColor");
    }

    raytracer::object::primitive::SurfaceData FlatColor::surfaceData(const maths::Vector& hitPoint) const {
        auto data = _wrapped->surfaceData(hitPoint);

        data.color = _color;
        data.emission = _emission;
        data.reflType = _refl;

        return data;
    }

} // namespace raytracer::object::material
