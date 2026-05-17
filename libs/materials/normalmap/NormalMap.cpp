/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** NormalMap
*/

#include "NormalMap.hpp"

#include <cmath>

#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material::detail {
    struct OrthonormalBasis {
        maths::Vector tangent;
        maths::Vector bitangent;
    };

    OrthonormalBasis buildBasis(const maths::Vector& normal) {
        const maths::Vector reference = std::fabs(normal.y) < 0.999
                                            ? maths::Vector(0, 1, 0)
                                            : maths::Vector(1, 0, 0);
        const maths::Vector tangent = reference.cross(normal).normalized();
        const maths::Vector bitangent = normal.cross(tangent).normalized();
        return {tangent, bitangent};
    }
}  // namespace raytracer::object::material::detail

namespace raytracer::object::material {
    NormalMap::NormalMap(const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _normalMapPath(util::ObjectMiddleware::optional<std::string>(
              args, "normalMapPath", std::string{}, "NormalMap")),
          _color(util::Helpers::optionalColor(
              args, "color", maths::Color(255, 255, 255), "NormalMap")),
          _scale(util::ObjectMiddleware::optional<double>(args, "scale", 1.0,
                                                          "NormalMap")),
          _strength(util::ObjectMiddleware::optional<double>(
              args, "strength", 1.0, "NormalMap")) {
        if (_normalMapPath.empty()) {
            _normalMapPath = util::ObjectMiddleware::optional<std::string>(
                args, "texturePath", std::string{}, "NormalMap");
        }
        if (_normalMapPath.empty()) {
            _normalMapPath = util::ObjectMiddleware::optional<std::string>(
                args, "path", std::string{}, "NormalMap");
        }
        if (_normalMapPath.empty()) {
            throw exception::PluginException(
                "NormalMap requires parameter 'normalMapPath'");
        }

        util::Helpers::unsignedDouble(_scale, "scale", "NormalMap");
        if (_scale <= 0.0) {
            _scale = 1.0;
        }

        util::Helpers::unsignedDouble(_strength, "strength", "NormalMap");
        if (_strength < 0.0) {
            _strength = 0.0;
        }

        preloadTexture(_normalMapPath);
    }

    void NormalMap::preloadTexture(const std::string& path) {
        if (_loadedTextures.contains(path)) {
            return;
        }

        sf::Image image;
        if (!image.loadFromFile(path)) {
            throw exception::PluginException("Failed to load normal map: {}",
                                             path);
        }
        _loadedTextures[path] = image;
    }

    maths::Vector NormalMap::sampleNormalMap(const maths::Vector& uv) const {
        const auto it = _loadedTextures.find(_normalMapPath);
        if (it == _loadedTextures.end()) {
            return maths::Vector(0, 0, 1);
        }

        const sf::Image& img = it->second;
        const unsigned int width = img.getSize().x;
        const unsigned int height = img.getSize().y;
        if (width == 0 || height == 0) {
            return maths::Vector(0, 0, 1);
        }

        const double scaledU = uv.x / _scale;
        const double scaledV = uv.y / _scale;
        const double u = scaledU - std::floor(scaledU);
        const double v = scaledV - std::floor(scaledV);

        const unsigned int x = static_cast<unsigned int>(u * (width - 1));
        const unsigned int y =
            static_cast<unsigned int>((1.0 - v) * (height - 1));

        sf::Color pixel;
#if SFML_VERSION_MAJOR >= 3
        pixel = img.getPixel(sf::Vector2u(x, y));
#else
        pixel = img.getPixel(x, y);
#endif

        const maths::Vector tangentSpaceNormal(
            (static_cast<double>(pixel.r) / 255.0) * 2.0 - 1.0,
            (static_cast<double>(pixel.g) / 255.0) * 2.0 - 1.0,
            (static_cast<double>(pixel.b) / 255.0) * 2.0 - 1.0);

        if (tangentSpaceNormal.magnitude() == 0.0) {
            return maths::Vector(0, 0, 1);
        }
        return tangentSpaceNormal.normalized();
    }

    maths::Vector NormalMap::toWorldNormal(
        const maths::Vector& tangentSpaceNormal,
        const maths::Vector& baseNormal) const {
        const auto basis = detail::buildBasis(baseNormal);
        maths::Vector adjustedTangentNormal(tangentSpaceNormal.x * _strength,
                                            tangentSpaceNormal.y * _strength,
                                            tangentSpaceNormal.z);

        if (adjustedTangentNormal.magnitude() == 0.0) {
            return baseNormal.normalized();
        }

        adjustedTangentNormal = adjustedTangentNormal.normalized();

        maths::Vector worldNormal =
            basis.tangent * adjustedTangentNormal.x +
            basis.bitangent * adjustedTangentNormal.y +
            baseNormal.normalized() * adjustedTangentNormal.z;

        if (worldNormal.magnitude() == 0.0) {
            return baseNormal.normalized();
        }

        worldNormal = worldNormal.normalized();
        if (worldNormal.dot(baseNormal) < 0.0) {
            worldNormal = worldNormal * -1;
        }
        return worldNormal;
    }

    primitive::MaterialProperties NormalMap::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        (void)hitPoint;

        const maths::Vector baseNormal = data.normal.magnitude() == 0.0
                                             ? maths::Vector(0, 0, 1)
                                             : data.normal.normalized();
        const maths::Vector tangentSpaceNormal = sampleNormalMap(data.uv);
        const maths::Vector perturbedNormal =
            toWorldNormal(tangentSpaceNormal, baseNormal);

        return {.color = _color,
                .emission = _emission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness,
                .perturbedNormal = perturbedNormal};
    }

}  // namespace raytracer::object::material
