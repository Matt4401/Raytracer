/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

int main() {
    raytracer::Core core;

    try {
        core.init();
    } catch (const std::exception &err) {
        std::cerr << "Error: " << err.what() << "\n";
    }
    core.run();
    return 0;
}
