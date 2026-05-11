/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** CrackedDirt
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "object/material/AWorleyBasedMaterial.hpp"

namespace raytracer::object::material {
    class CrackedDirt : public AWorleyBasedMaterial {
      public:
        explicit CrackedDirt(const std::map<std::string, std::any>& args);
        ~CrackedDirt() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;
    };
}  // namespace raytracer::object::material
