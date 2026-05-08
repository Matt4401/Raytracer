/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Chessboard
*/

#include "Chessboard.hpp"

#include <cmath>

#include "exception/ParsingException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    Chessboard::Chessboard(const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _color1(util::Helpers::toColor(args, "color1", "Chessboard")),
          _color2(util::Helpers::toColor(args, "color2", "Chessboard")),
          _tileSize(0.0) {
        _tileSize = util::ObjectMiddleware::validate<double>(args, "tileSize",
                                                             "Chessboard");
        util::Helpers::unsignedDouble(_tileSize, "tileSize", "Chessboard");
        if (_tileSize <= 0.0)
            throw raytracer::exception::ParsingException(
                "Chessboard: tileSize must be positive");
    }

    primitive::MaterialProperties Chessboard::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double uCoord = data.uv.x * _tileSize;
        double vCoord = data.uv.y * _tileSize;

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
