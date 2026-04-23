/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "object/IObject.hpp"

extern "C" __attribute__((constructor)) void constructor(void) {
    std::cout << "Sphere Plugin loaded successfully." << std::endl;
}

extern "C" __attribute__((destructor)) void destructor(void) {
    std::cout << "Sphere Plugin unloaded successfully." << std::endl;
}

extern "C" std::string type(void) {
    return "primitive";
}

extern "C" std::string name(void) {
    return "sphere";
}

extern "C" std::function<object::IObject*(const std::vector<std::any>&)>
builder(void) {
    return [](const std::vector<std::any>& args) -> object::IObject* {
        // Implementation for creating a Sphere object
        return nullptr;
    };
}