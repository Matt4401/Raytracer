/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpotLightPlugin
*/

#include <any>
#include <map>
#include <string>

#include "SpotLight.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *spotLightBuilder(
    const std::map<std::string, std::any> &params) {
    return new raytracer::object::light::SpotLight(params);
}

extern "C" const char *type(void) {
    return "light";
}

extern "C" const char *name(void) {
    return "spot_light";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &spotLightBuilder;
}
