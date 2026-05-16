/*
** EPITECH PROJECT, 2026
** ExportPPM
** File description:
** ExportPPM code
*/

#include "ExportPPM.hpp"

#include <iostream>

#include "object/ICamera.hpp"
#include "object/IScene.hpp"

namespace raytracer::exporter {

    void ExportPPM::writeColor(const maths::Color color) {
        std::cout << static_cast<int>(color.r) << " "
                  << static_cast<int>(color.g) << " "
                  << static_cast<int>(color.b) << "\n";
    }

    void ExportPPM::writeFile(const raytracer::object::scene::IScene &scene,
                              const std::vector<maths::Color> &pixels) const {
        const auto &camera = scene.cameras().at(0);
        int imageWidth = camera->imageWidth();
        int imageHeight = camera->imageHeight();
        std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
        for (const auto &col : pixels) writeColor(col);
    }

}  // namespace raytracer::exporter
