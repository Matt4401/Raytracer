/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** main
*/

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Core.hpp"

int main(int argc, char **argv) {
    raytracer::Core core;
    std::vector<std::string> arguments(argv + 1, argv + argc);

    if (auto state = core.helpMessage(arguments); state.first) {
        return state.second;
    }

    try {
        core.init(arguments);
    } catch (const std::exception &err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 84;
    }
    core.run();
    return 0;
}
