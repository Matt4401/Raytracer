/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AMaterial
*/

#pragma once

#include <cstdint>

#include "object/AObject.hpp"
#include "object/material/IMaterial.hpp"

namespace raytracer::object::material {
    class AMaterial : public IMaterial, public AObject {
      public:
        AMaterial() : AObject(Type::MATERIAL) {
        }
        ~AMaterial() override = default;

      protected:
        static constexpr double K_REFR_TRANSPARENCY_THRESHOLD = 0.85;
        static constexpr double K_REFR_IOR_THRESHOLD = 1.01;
        static constexpr double K_SPEC_REFLECTIVITY_THRESHOLD = 0.6;
        static constexpr double K_SPEC_METALNESS_THRESHOLD = 0.5;

        static primitive::RefltT chooseReflTypeFromParams(double reflectivity,
                                                          double transparency,
                                                          double ior,
                                                          double metalness);
    };
}  // namespace raytracer::object::material
