/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** NormalMap
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <any>
#include <map>
#include <string>
#include <unordered_map>

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class NormalMap : public ABasicMaterial {
      public:
        explicit NormalMap(const std::map<std::string, std::any>& args);
        ~NormalMap() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) const override;

      private:
        std::string _normalMapPath;
        maths::Color _color;
        double _scale;
        double _strength;
        mutable std::unordered_map<std::string, sf::Image> _loadedTextures;

        void preloadTexture(const std::string& path);
        maths::Vector sampleNormalMap(const maths::Vector& uv) const;
        maths::Vector toWorldNormal(const maths::Vector& tangentSpaceNormal,
                                    const maths::Vector& baseNormal) const;
    };
}  // namespace raytracer::object::material
