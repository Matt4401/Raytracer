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
                int done = render.getNbWorkerDone(activeWorkers);
                double pct = render.imageSize().heigth == 0
                                 ? 100.0
                                 : (100.0 * done) / render.imageSize().heigth;

                std::cerr << "\rRender " << (int)pct << "% ";
                std::cerr << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            std::cerr << "\rRender 100%\n";
        });
    }

}  // namespace raytracer::visual
