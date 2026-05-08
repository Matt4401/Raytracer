/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Chessboard
*/

#include "Chessboard.hpp"

#include <cmath>

#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    Chessboard::Chessboard(const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _color1(util::Helpers::toColor(args, "color1", "Chessboard")),
          _color2(util::Helpers::toColor(args, "color2", "Chessboard")),
          _frequency(0.0) {
        _frequency = util::ObjectMiddleware::validate<double>(args, "tileSize",
                                                             "Chessboard");
        util::Helpers::unsignedDouble(_frequency, "tileSize", "Chessboard");
        if (_frequency <= 0.0)
            throw exception::PluginException(
                "Chessboard: tileSize must be positive");
    }

    primitive::MaterialProperties Chessboard::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double uCoord = data.uv.x * _frequency;
        double vCoord = data.uv.y * _frequency;

        int checkX = static_cast<int>(std::floor(uCoord));
        int checkY = static_cast<int>(std::floor(vCoord));

        int checkerboard = std::abs(checkX + checkY) % 2;

        maths::Color finalColor = (checkerboard == 0) ? _color1 : _color2;

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
