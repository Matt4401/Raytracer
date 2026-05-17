/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** NormalMapPlugin
*/

#include <any>
#include <map>
#include <string>

#include "NormalMap.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::MATERIAL;
}

extern "C" const char *name(void) {
    return "normalmap";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::material::NormalMap(params);
    };
}
