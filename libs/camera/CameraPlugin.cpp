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

extern "C" raytracer::object::IObject *CameraBuilder(
    const std::map<std::string, std::any> &params) {
    return new raytracer::object::camera::Camera(params);
}

extern "C" const char *type(void) {
    return "camera";
}

extern "C" const char *name(void) {
    return "camera";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return &CameraBuilder;
}
