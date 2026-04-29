/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** FlatColorPlugin
*/

#include <any>
#include <vector>

#include "FlatColor.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *flatcolorBuilder(
    const std::vector<std::any> &args) {
    return new raytracer::object::material::FlatColor(args);
}

extern "C" const char *type(void) {
    return "flatcolor";
}
