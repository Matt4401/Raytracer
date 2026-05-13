/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"

int main(int argc, char **argv) {
    raytracer::Core core;

    if (auto state = core.helpMessage(argc, argv); state.first) {
        return state.second;
    }

    try {
        core.init(argv[1]);
    } catch (const std::exception &err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 84;
    }
    core.run();
    return 0;
}
