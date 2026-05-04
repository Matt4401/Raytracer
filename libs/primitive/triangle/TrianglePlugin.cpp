/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** TrianglePlugin
*/

#include <any>
#include <map>
#include <string>

#include "Triangle.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "triangle";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::primitive::Triangle(params);
    };
}
