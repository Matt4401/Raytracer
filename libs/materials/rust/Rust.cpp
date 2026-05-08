/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Rust
*/

#include "Rust.hpp"

#include <cmath>

namespace raytracer::object::material {
    Rust::Rust(const std::map<std::string, std::any>& args)
        : APerlinBasedMaterial(args) {
    }

    primitive::MaterialProperties Rust::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        double noise = calculateFBM(hitPoint.x * _scale, hitPoint.y * _scale, hitPoint.z * _scale);
        double n = (noise + 1.0) * 0.5;

        maths::Color finalColor;
        primitive::RefltT localRefl;
        double localReflectivity;
        double localRoughness;
        double localMetalness;

        if (n > 0.53) {
            localRefl = raytracer::object::primitive::RefltT::SPEC;
            localReflectivity = 0.5;
            localRoughness = 0.3;
            localMetalness = 1.0;
            finalColor = _color1;
        } else {
            localRefl = raytracer::object::primitive::RefltT::DIFF;
            localReflectivity = 0.0;
            localRoughness = 0.9;
            localMetalness = 0.0;
            finalColor = _color2;
        }

        return {.color = finalColor,
                .emission = _emission,
                .reflType = localRefl,
                .reflectivity = localReflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = localRoughness,
                .metalness = localMetalness};
    }

}  // namespace raytracer::object::material