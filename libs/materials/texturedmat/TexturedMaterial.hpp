/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TexturedMaterial
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "MtlLoader.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/AMaterial.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::material {
    class TexturedMaterial : public AMaterial {
      public:
        explicit TexturedMaterial(const std::map<std::string, std::any>& args);
        ~TexturedMaterial() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        maths::Color _color;
        maths::Vector _emission;
        primitive::RefltT _refl;
        double _reflectivity;
        double _transparency;
        double _ior;
        double _roughness;
        double _metalness;
        std::string _texturePath;
        std::unique_ptr<MtlLoader> _materialLoader;
    };
}  // namespace raytracer::object::material
