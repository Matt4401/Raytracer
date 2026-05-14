/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CylinderPlugin
*/

#include <any>
#include <map>
#include <string>

#include "Cylinder.hpp"
#include "LimitedCylinder.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "cylinder";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        // height == limit cylinder
        if (params.contains("height")) {
            return new raytracer::object::primitive::LimitedCylinder(params);
        }
        return new raytracer::object::primitive::Cylinder(params);
    };
}
