/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

#include <any>
#include <map>

#include "./ValidObject.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *testBuilder(
    const std::map<std::string, std::any> &args) {
    return new raytracer::tests::ValidObject();
}

extern "C" const char *type(void) {
    return "primitive";
}

extern "C" const char *name(void) {
    return "valid_test";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &testBuilder;
}
