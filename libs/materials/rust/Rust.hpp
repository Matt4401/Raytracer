/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Rust
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/APerlinBasedMaterial.hpp"

namespace raytracer::object::material {
    class Rust : public APerlinBasedMaterial {
      public:
        explicit Rust(const std::map<std::string, std::any>& args);
        ~Rust() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
