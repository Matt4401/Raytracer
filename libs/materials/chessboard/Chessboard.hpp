/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Chessboard
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class Chessboard : public ABasicMaterial {
      public:
        explicit Chessboard(const std::map<std::string, std::any>& args);
        ~Chessboard() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color1;
        maths::Color _color2;
        double _tileSize;
    };
}  // namespace raytracer::object::material
