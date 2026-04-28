/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

#include <any>
#include <vector>

#include "Sphere.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *sphereBuilder(
    const std::vector<std::any> &args) {
    return new raytracer::object::primitive::Sphere(args);

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
