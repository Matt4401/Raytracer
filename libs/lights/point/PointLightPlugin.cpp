/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** PointLightPlugin
*/

#include <any>
#include <map>
#include <string>

#include "PointLight.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::IObject *pointLightBuilder(
    const std::map<std::string, std::any> &params) {
    return new raytracer::object::light::PointLight(params);
}

extern "C" const char *type(void) {
    return "light";
}

extern "C" const char *name(void) {
    return "point_light";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &pointLightBuilder;
}
