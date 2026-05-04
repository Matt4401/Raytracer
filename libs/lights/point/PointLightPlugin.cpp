/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** PointLightPlugin
*/

#include <any>
#include <map>
#include <string>

#include "PointLight.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::LIGHT;
}

extern "C" const char *name(void) {
    return "point_light";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::light::PointLight(params);
    };
}
