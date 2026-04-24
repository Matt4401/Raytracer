/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MissingNamePlugin
*/

#include <any>
#include <vector>

#include "object/IObject.hpp"

extern "C" const char *type(void) {
    return "primitive";
}

extern "C" raytracer::object::IObject *testBuilder(
    const std::vector<std::any> &args) {
    (void)args;
    return nullptr;
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return &testBuilder;
}
