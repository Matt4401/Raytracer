/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TuringPattern
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class TuringPattern : public APerlinBasedMaterial {
      public:
        explicit TuringPattern(const std::map<std::string, std::any>& args);
        ~TuringPattern() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
      private:
          double _offset;
          double _threshold;
    };
}  // namespace raytracer::object::material
