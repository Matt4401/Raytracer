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

__attribute__((constructor)) extern "C" void constructor(void) {
    std::cout << "Sphere Plugin loaded successfully." << std::endl;
}

__attribute__((destructor)) extern "C" void destructor(void) {
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