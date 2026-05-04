/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** CameraPlugin
*/

#include <any>
#include <map>
#include <string>

#include "Camera.hpp"
#include "object/IObject.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::CAMERA;
}

extern "C" const char *name(void) {
    return "camera";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::camera::Camera(params);
    };
}
