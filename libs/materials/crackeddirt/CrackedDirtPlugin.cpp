/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** CrackedDirtPlugin
*/

#include <any>
#include <map>
#include <string>

#include "CrackedDirt.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::MATERIAL;
}

extern "C" const char *name(void) {
    return "cracked_dirt";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::material::CrackedDirt(params);
    };
}
