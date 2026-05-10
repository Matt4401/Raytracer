/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Caustics
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/AWorleyBasedMaterial.hpp"

namespace raytracer::object::material {
    class Caustics : public AWorleyBasedMaterial {
      public:
        explicit Caustics(const std::map<std::string, std::any>& args);
        ~Caustics() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
