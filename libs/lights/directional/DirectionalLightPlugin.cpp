/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLightPlugin
*/

#include <any>
#include <map>
#include <string>

#include "DirectionalLight.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::Type type(void) {
    return raytracer::object::Type::LIGHT;
}

extern "C" const char *name(void) {
    return "directional_light";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::light::DirectionalLight(params);
    };
}
