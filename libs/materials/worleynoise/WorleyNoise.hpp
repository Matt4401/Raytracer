/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** WorleyNoise
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/AWorleyBasedMaterial.hpp"

namespace raytracer::object::material {
    class WorleyNoise : public AWorleyBasedMaterial {
      public:
        explicit WorleyNoise(const std::map<std::string, std::any>& args);
        ~WorleyNoise() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
