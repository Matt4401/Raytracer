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

extern "C" raytracer::object::Type type(void) {
    return raytracer::object::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "sphere";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::primitive::Sphere(params);
    };
}
