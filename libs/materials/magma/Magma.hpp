/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Magma
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class Magma : public APerlinBasedMaterial {
      public:
        explicit Magma(const std::map<std::string, std::any>& args);
        ~Magma() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
