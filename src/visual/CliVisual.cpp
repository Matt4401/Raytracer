/*
** EPITECH PROJECT, 2026
** CliVisual
** File description:
** CliVisual code
*/

#include "visual/CliVisual.hpp"

#include <iostream>
#include <thread>

#include "Render.hpp"

namespace raytracer::visual {

    std::thread CliVisual::printProgress(int activeWorkers, Render &render) {
        return std::thread([&render, activeWorkers]() {
            while (!render.renderingIsFinished()) {
                std::cerr << "\rRender "
                          << render.getPercentRendered(activeWorkers) << "% ";
                std::cerr << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            std::cerr << "\rRender 100%\n";
        });
    }

    bool CliVisual::allowPreview() {
        return false;
    }

    bool CliVisual::fullRender() {
        return true;
    }

}  // namespace raytracer::visual
