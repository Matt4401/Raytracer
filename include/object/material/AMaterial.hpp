/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AMaterial
*/

#pragma once

#include "object/AObject.hpp"
#include "object/material/IMaterial.hpp"

namespace raytracer::object::material {
    class AMaterial : public IMaterial, public AObject {
      public:
        AMaterial() : AObject(Type::MATERIAL) {
        }
        ~AMaterial() override = default;
    };
}  // namespace raytracer::object::material
