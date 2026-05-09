/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ABasicMaterial
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Vector.hpp"
#include "object/material/AMaterial.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::material {
    class ABasicMaterial : public AMaterial {
      public:
        explicit ABasicMaterial(const std::map<std::string, std::any>& args);
        ~ABasicMaterial() override = default;

      protected:
        maths::Vector _emission;
        primitive::RefltT _refl;
        double _reflectivity;
        double _transparency;
        double _ior;
        double _roughness;
        double _metalness;
    };
}  // namespace raytracer::object::material
