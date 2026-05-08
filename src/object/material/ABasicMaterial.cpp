/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ABasicMaterial
*/

#include "object/material/ABasicMaterial.hpp"

#include <algorithm>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    ABasicMaterial::ABasicMaterial(const std::map<std::string, std::any>& args)
        : AMaterial(),
          _emission(0, 0, 0),
          _refl(util::ObjectMiddleware::validate<
                raytracer::object::primitive::RefltT>(args, "reflType",
                                                      "Material")),
          _reflectivity(0.0),
          _transparency(0.0),
          _ior(1.0),
          _roughness(0.0),
          _metalness(0.0) {
        _emission = util::Helpers::optionalVector(args, "emission", _emission,
                                                  "Material");
        _reflectivity = util::ObjectMiddleware::optional<double>(
            args, "reflectivity", 0.0, "Material");
        util::Helpers::unsignedDouble(_reflectivity, "reflectivity",
                                      "Material");
        _reflectivity = std::min(_reflectivity, 1.0);

        _transparency = util::ObjectMiddleware::optional<double>(
            args, "transparency", 0.0, "Material");
        util::Helpers::unsignedDouble(_transparency, "transparency",
                                      "Material");
        _transparency = std::min(_transparency, 1.0);

        _ior = util::ObjectMiddleware::optional<double>(args, "ior", 1.0,
                                                        "Material");
        util::Helpers::unsignedDouble(_ior, "ior", "Material");
        if (_ior <= 0.0)
            _ior = 1.0;

        _roughness = util::ObjectMiddleware::optional<double>(args, "roughness",
                                                              0.0, "Material");
        util::Helpers::unsignedDouble(_roughness, "roughness", "Material");
        _roughness = std::min(_roughness, 1.0);

        _metalness = util::ObjectMiddleware::optional<double>(args, "metalness",
                                                              0.0, "Material");
        util::Helpers::unsignedDouble(_metalness, "metalness", "Material");
        _metalness = std::min(_metalness, 1.0);
    }

}  // namespace raytracer::object::material
