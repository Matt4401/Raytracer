/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APerlinBasedMaterial
*/

#include "object/material/APerlinBasedMaterial.hpp"

#include "math/Noise.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {
    APerlinBasedMaterial::APerlinBasedMaterial(
        const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _color1(
              util::Helpers::toColor(args, "color1", "APerlinBasedMaterial")),
          _color2(
              util::Helpers::toColor(args, "color2", "APerlinBasedMaterial")) {
        _scale = util::ObjectMiddleware::optional<double>(
            args, "scale", 1.0, "APerlinBasedMaterial");
        util::Helpers::unsignedDouble(_scale, "scale", "APerlinBasedMaterial");
        if (_scale <= 0.0)
            _scale = 1.0;

        _octaves = util::ObjectMiddleware::optional<int>(
            args, "octaves", 1, "APerlinBasedMaterial");
        if (_octaves < 1)
            _octaves = 1;

        _persistence = util::ObjectMiddleware::optional<double>(
            args, "persistence", 0.5, "APerlinBasedMaterial");
        util::Helpers::unsignedDouble(_persistence, "persistence",
                                      "APerlinBasedMaterial");
        if (_persistence <= 0.0)
            _persistence = 0.5;

        _turbulence = util::ObjectMiddleware::optional<double>(
            args, "turbulence", 0.0, "APerlinBasedMaterial");
        util::Helpers::unsignedDouble(_turbulence, "turbulence",
                                      "APerlinBasedMaterial");
    }

    double APerlinBasedMaterial::calculateFBM(double x, double y,
                                              double z) const {
        double amplitude = 1.0;
        double frequency = 1.0;
        double noise = 0.0;
        double maxValue = 0.0;

        for (int i = 0; i < _octaves; ++i) {
            noise += maths::Noise::perlin(x * frequency, y * frequency,
                                          z * frequency) *
                     amplitude;
            maxValue += amplitude;
            amplitude *= _persistence;
            frequency *= 2.0;
        }
        return noise / maxValue;
    }
}  // namespace raytracer::object::material