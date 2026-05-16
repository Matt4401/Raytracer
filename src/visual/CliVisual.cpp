/*
** EPITECH PROJECT, 2026
** CliVisual
** File description:
** CliVisual code
*/

#include "visual/CliVisual.hpp"

#include <cstddef>
#include <iostream>
#include <iterator>
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

    bool CliVisual::wantSave(Render &render) {
        (void)render;
        return true;
    }

    bool CliVisual::stopLoop() {
        return this->_stop;
    }

    std::string CliVisual::selectScene(scenesMap &scene,
                                       [[maybe_unused]] Render &render) {
        return std::next(scene.begin(), 0)->first;
    }

}  // namespace raytracer::visual
