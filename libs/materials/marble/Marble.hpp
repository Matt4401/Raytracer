/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Marble
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class Marble : public APerlinBasedMaterial {
      public:
        explicit Marble(const std::map<std::string, std::any>& args);
        ~Marble() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
