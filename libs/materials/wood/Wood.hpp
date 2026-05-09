/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Wood
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class Wood : public APerlinBasedMaterial {
      public:
        explicit Wood(const std::map<std::string, std::any>& args);
        ~Wood() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
