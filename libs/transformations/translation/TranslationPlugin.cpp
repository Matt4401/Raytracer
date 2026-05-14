/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TranslationPlugin
*/

#include <any>
#include <map>
#include <string>

#include "Translation.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::TRANSFORM;
}

extern "C" const char *name(void) {
    return "translation";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::primitive::Translation(params);
    };
}
