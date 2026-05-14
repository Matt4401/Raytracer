/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConePlugin
*/

#include <any>
#include <map>
#include <string>

#include "Cone.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "cone";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        // heigth == limit Cone
        if (params.contains("height")) {
            return new raytracer::object::primitive::LimitedCone(params);
        }
        return new raytracer::object::primitive::Cone(params);
    };
}
