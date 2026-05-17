/*
** EPITECH PROJECT, 2026
** IExport
** File description:
** IExport header
*/

#pragma once

#include <vector>

#include "object/IScene.hpp"

namespace raytracer::exporter {

    class IExport {
      public:
        virtual ~IExport() = default;

        /**
         * @brief Export a rendered scene as pixel data to a file.
         *
         * This method takes the rendered pixel data from a scene and writes it
         * to a file in the format specified by the implementing class. The
         * pixel vector should be ordered sequentially from top-left to
         * bottom-right, row by row, matching the scene's image dimensions.
         *
         * @param scene The scene that was rendered. Used to retrieve metadata
         *              such as image width and height.
         * @param pixels A vector of Color objects representing the rendered
         *               pixels. The size should match width * height of the
         *               scene's image dimensions.
         */
        virtual void writeFile(
            const raytracer::object::scene::IScene &scene,
            const std::vector<maths::Color> &pixels) const = 0;
    };

}  // namespace raytracer::exporter
