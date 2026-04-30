/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ScenePlugin
*/

#include "ScenePlugin.hpp"

extern "C" raytracer::object::Type type(void) {
    return raytracer::object::Type::SCENE;
}

extern "C" const char *name(void) {
    return "scene";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        return new raytracer::object::scene::Scene(params);
    };
}