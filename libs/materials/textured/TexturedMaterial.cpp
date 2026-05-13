/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** TexturedMaterial
*/

#include "TexturedMaterial.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>

#include "exception/ParsingException.hpp"
#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {
    TexturedMaterial::TexturedMaterial(
        const std::map<std::string, std::any>& args)
        : ABasicMaterial(args) {
        try {
            auto mtlPath = util::ObjectMiddleware::validate<std::string>(
                args, "mtlPath", "TexturedMaterial");
            handleMTL(mtlPath, args);
        } catch (const exception::PluginException& z) {
            _texturePath = util::ObjectMiddleware::validate<std::string>(
                args, "texturePath", "TexturedMaterial");
            handleTexture(args);
        }
    }

    void TexturedMaterial::preloadTexture(const std::string& path) const {
        if (!_loadedTextures.contains(path)) {
            sf::Image img;
            if (img.loadFromFile(path)) {
                _loadedTextures[path] = img;
            } else if (img.loadFromFile(std::filesystem::path(
                           "assets/images/missingTexture.png"))) {
                _loadedTextures[path] = img;
            } else {
                throw exception::PluginException("Failed to load texture: " +
                                                 path);
            }
        }
    }

    void TexturedMaterial::handleTexture(
        const std::map<std::string, std::any>& args) {
        try {
            _scale = util::ObjectMiddleware::optional<double>(
                args, "scale", 1.0, "TexturedMaterial");
            util::Helpers::unsignedDouble(_scale, "scale", "TexturedMaterial");
            if (_scale <= 0.0)
                _scale = 1.0;

            if (!_texturePath.empty()) {
                preloadTexture(_texturePath);
            }
        } catch (const exception::PluginException& e2) {
            throw exception::ParsingException(e2.what());
        }
    }

    void TexturedMaterial::handleMTL(
        const std::string& mtlPath,
        const std::map<std::string, std::any>& args) {
        try {
            _materialLoader = std::make_unique<MtlLoader>(mtlPath);
            for (const auto& [name, mat] : _materialLoader->materials()) {
                if (!mat.mapKd().empty()) {
                    preloadTexture(mat.mapKd());
                }
            }
            _scale = util::ObjectMiddleware::optional<double>(
                args, "scale", 1.0, "TexturedMaterial");
            util::Helpers::unsignedDouble(_scale, "scale", "TexturedMaterial");
            if (_scale <= 0.0)
                _scale = 1.0;
        } catch (const std::exception& e) {
            throw exception::ParsingException(e.what());
        }
    }

    maths::Color TexturedMaterial::sampleTexture(
        const std::string& path, const maths::Vector& uv) const {
        auto it = _loadedTextures.find(path);
        if (it == _loadedTextures.end()) {
            try {
                preloadTexture(path);
            } catch (const std::exception&) {
                return maths::Color(255, 0, 255);
            }
            it = _loadedTextures.find(path);
            if (it == _loadedTextures.end())
                return maths::Color(255, 0, 255);
        }

        const sf::Image& img = it->second;
        unsigned int width = img.getSize().x;
        unsigned int height = img.getSize().y;

        if (width == 0 || height == 0)
            return maths::Color(255, 0, 0);

        double scaledU = uv.x / _scale;
        double scaledV = uv.y / _scale;

        double u = scaledU - std::floor(scaledU);
        double v = scaledV - std::floor(scaledV);

        unsigned int x = static_cast<unsigned int>(u * (width - 1));
        unsigned int y = static_cast<unsigned int>((1.0 - v) * (height - 1));

        sf::Color sfCol;
#if SFML_VERSION_MAJOR >= 3
        sfCol = img.getPixel(sf::Vector2u(x, y));
#else
        sfCol = img.getPixel(x, y);
#endif
        return maths::Color(sfCol.r, sfCol.g, sfCol.b);
    }

    primitive::MaterialProperties TexturedMaterial::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        if (_materialLoader && !_materialLoader->materials().empty() &&
            !data.materialName.empty()) {
            const auto& mat = _materialLoader->get(data.materialName);

            maths::Color finalColor;
            if (mat.mapKd().empty()) {
                finalColor = maths::Color(mat.kd().x, mat.kd().y, mat.kd().z);
            } else {
                finalColor = sampleTexture(mat.mapKd(), data.uv);
            }

            return {
                .color = finalColor,
                .emission = mat.ke(),
                .reflType = primitive::RefltT::DIFF,
                .reflectivity = mat.ns() / 1000.0,
                .transparency = mat.d(),
                .ior = mat.ni(),
                .roughness =
                    std::clamp(1.0 - std::sqrt(mat.ns() / 1000.0), 0.0, 1.0),
                .metalness = std::clamp(
                    (mat.ks().x + mat.ks().y + mat.ks().z) / 3.0, 0.0, 1.0)};
        } else {
            maths::Color finalColor = sampleTexture(_texturePath, data.uv);
            return {.color = finalColor,
                    .emission = _emission,
                    .reflType = _refl,
                    .reflectivity = _reflectivity,
                    .transparency = _transparency,
                    .ior = _ior,
                    .roughness = _roughness,
                    .metalness = _metalness};
        }
    }
}  // namespace raytracer::object::material