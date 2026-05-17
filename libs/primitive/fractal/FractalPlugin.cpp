/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** FractalPlugin
*/

#include <any>
#include <map>
#include <object/AObject.hpp>
#include <string>

#include "fractal/FractalBuilder.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "fractal";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return raytracer::object::primitive::buildFractal(params,
                                                          "FractalPlugin");
    };
}
