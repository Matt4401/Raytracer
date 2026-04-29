/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

#include <any>
#include <map>
#include <string>

#include "Sphere.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *sphereBuilder(
    const std::map<std::string, std::any> &params) {
    return new raytracer::object::primitive::Sphere(params);
}

extern "C" const char *type(void) {
    return "primitive";
}

extern "C" const char *name(void) {
    return "sphere";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &sphereBuilder;
}
