/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLightPlugin
*/

#include <any>
#include <map>
#include <string>

#include "DirectionalLight.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *directionalLightBuilder(
    const std::map<std::string, std::any> &params) {
    return new raytracer::object::light::DirectionalLight(params);
}

extern "C" const char *type(void) {
    return "light";
}

extern "C" const char *name(void) {
    return "directional_light";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &directionalLightBuilder;
}
