/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AWorleyBasedMaterial
*/

#include "object/material/AWorleyBasedMaterial.hpp"

#include "math/Noise.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {
    AWorleyBasedMaterial::AWorleyBasedMaterial(
        const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _color1(
              util::Helpers::toColor(args, "color1", "AWorleyBasedMaterial")),
          _color2(
              util::Helpers::toColor(args, "color2", "AWorleyBasedMaterial")) {
        _scale = util::ObjectMiddleware::optional<double>(
            args, "scale", 1.0, "AWorleyBasedMaterial");
        util::Helpers::unsignedDouble(_scale, "scale", "AWorleyBasedMaterial");
        if (_scale <= 0.0)
            _scale = 1.0;
    }

    std::pair<double, double> AWorleyBasedMaterial::getWorley(double x,
                                                              double y,
                                                              double z) const {
        return maths::Noise::worley(x, y, z);
    }
}  // namespace raytracer::object::material