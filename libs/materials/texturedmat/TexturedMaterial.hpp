/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TexturedMaterial
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <any>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "MtlLoader.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/material/ABasicMaterial.hpp"

namespace raytracer::object::material {
    class TexturedMaterial : public ABasicMaterial {
      public:
        explicit TexturedMaterial(const std::map<std::string, std::any>& args);
        ~TexturedMaterial() override = default;

        primitive::MaterialProperties evaluate(
            const primitive::SurfaceData& data,
            const maths::Vector& hitPoint) override;

      private:
        std::string _texturePath;
        double _scale;
        std::unique_ptr<MtlLoader> _materialLoader;
        std::unordered_map<std::string, sf::Image> _loadedTextures;

        maths::Color sampleTexture(const std::string& path,
                                   const maths::Vector& uv);

        void preloadTexture(const std::string& path);
    };
}  // namespace raytracer::object::material