/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** TexturedMaterial
*/

#include "TexturedMaterial.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {
    TexturedMaterial::TexturedMaterial(
        const std::map<std::string, std::any>& args)
        : AMaterial() {
        try {
            auto mtlPath = util::ObjectMiddleware::validate<std::string>(
                args, "mtlPath", "TexturedMaterial");
            _materialLoader = std::make_unique<MtlLoader>(mtlPath);
        } catch (const exception::PluginException&) {
            _materialLoader = nullptr;
            _color = util::Helpers::toColor(args, "color", "TexturedMaterial");
            _refl = util::ObjectMiddleware::validate<
                raytracer::object::primitive::RefltT>(args, "reflType",
                                                      "FlatColor");
            _emission = util::Helpers::optionalVector(args, "emission",
                                                      _emission, "FlatColor");
            _reflectivity = util::ObjectMiddleware::optional<double>(
                args, "reflectivity", 0.0, "FlatColor");
            util::Helpers::unsignedDouble(_reflectivity, "reflectivity",
                                          "FlatColor");
            _reflectivity = std::min(_reflectivity, 1.0);

            _transparency = util::ObjectMiddleware::optional<double>(
                args, "transparency", 0.0, "FlatColor");
            util::Helpers::unsignedDouble(_transparency, "transparency",
                                          "FlatColor");
            _transparency = std::min(_transparency, 1.0);

            _ior = util::ObjectMiddleware::optional<double>(args, "ior", 1.0,
                                                            "FlatColor");
            util::Helpers::unsignedDouble(_ior, "ior", "FlatColor");
            if (_ior <= 0.0)
                _ior = 1.0;

            _roughness = util::ObjectMiddleware::optional<double>(
                args, "roughness", 0.0, "FlatColor");
            util::Helpers::unsignedDouble(_roughness, "roughness", "FlatColor");
            _roughness = std::min(_roughness, 1.0);

            _metalness = util::ObjectMiddleware::optional<double>(
                args, "metalness", 0.0, "FlatColor");
            util::Helpers::unsignedDouble(_metalness, "metalness", "FlatColor");
            _metalness = std::min(_metalness, 1.0);

            _texturePath = util::ObjectMiddleware::validate<std::string>(
                args, "texturePath", "TexturedMaterial");
        }
    }

    primitive::MaterialProperties TexturedMaterial::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        if (_materialLoader->materials().empty()) {
            return {};
        }
        if (_materialLoader) {
            auto matName = util::ObjectMiddleware::validate<std::string>(
                data.extraParams, "materialName", "TexturedMaterial");
            const auto& mat = _materialLoader->materials().at(matName);

            if (mat.mapKd().empty()) {
                _color = maths::Color(mat.kd().x, mat.kd().y, mat.kd().z);
            } else {
                // TODO : load texture and sample color based on UV coordinates
                // color = sampleTexture(mat.mapKd(), data.uv);
            }
            _emission = mat.ke();
            _refl = primitive::RefltT::DIFF;  // TODO: derive reflection type
                                              // from material properties
            _reflectivity = mat.ns() / 1000.0;
            _transparency = mat.d();
            _ior = mat.ni();
            _roughness =
                0.5;  // TODO: derive roughness from material properties
            _metalness =
                0.0;  // TODO: derive metalness from material properties
        } else {
            // TODO: load Texture and sample color based on UV coordinates
            // _color = sampleTexture(_texturePath, data.uv);
        }
        return {.color = _color,
                .emission = _emission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness};
    }
}  // namespace raytracer::object::material