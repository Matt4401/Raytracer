/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AMaterial
*/

#include "object/material/AMaterial.hpp"

namespace raytracer::object::material {
    primitive::RefltT AMaterial::chooseReflTypeFromParams(
        const double reflectivity, const double transparency, const double ior,
        const double metalness) {
        if (transparency >= K_REFR_TRANSPARENCY_THRESHOLD &&
            ior > K_REFR_IOR_THRESHOLD) {
            return primitive::RefltT::REFR;
        }
        if (reflectivity >= K_SPEC_REFLECTIVITY_THRESHOLD ||
            metalness >= K_SPEC_METALNESS_THRESHOLD) {
            return primitive::RefltT::SPEC;
        }
        return primitive::RefltT::DIFF;
    }
}  // namespace raytracer::object::material
