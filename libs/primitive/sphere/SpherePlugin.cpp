/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

#include <any>
#include <string>
#include <vector>

#include "object/IObject.hpp"

extern "C" object::IObject* sphere_builder(const std::vector<std::any>& args) {
    return nullptr;
}

extern "C" const char* type(void) {
    return "primitive";
}

extern "C" const char* name(void) {
    return "sphere";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" void* builder(void) {
    return (void*)&sphere_builder;
}