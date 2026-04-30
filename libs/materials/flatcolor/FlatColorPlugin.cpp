/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** FlatColorPlugin
*/

#include <any>
#include <map>
#include <string>

#include "FlatColor.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *flatcolorBuilder(
    const std::map<std::string, std::any> &args) {
    return new raytracer::object::material::FlatColor(args);
}

extern "C" const char *type(void) {
    return "material";
}

extern "C" const char *name(void) {
    return "flatcolor";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return &flatcolorBuilder;
}
