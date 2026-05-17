/*
** EPITECH PROJECT, 2026
** ExportPPM
** File description:
** ExportPPM header
*/

#pragma once

#include "IExport.hpp"
namespace raytracer::exporter {

    class ExportPPM : public IExport {
      public:
        ExportPPM() = default;
        ~ExportPPM() override = default;

        /**
         * @brief Export a rendered scene to PPM format.
         *
         * This method writes the pixel data from the scene to a PPM format
         * file. The output is written to standard output (stdout) in ASCII
         * PPM format.
         *
         * The pixel data is written in row-major order (top to bottom,
         * left to right) with each color component (R, G, B) separated by
         * spaces.
         *
         * @param scene The scene that was rendered. The scene's image width
         *              and height are extracted to write the PPM header.
         * @param pixels A vector of Color objects representing the rendered
         *               pixels, ordered from top-left to bottom-right.
         *
         * @note Output is written to stdout, not to a file on disk.
         */
        void writeFile(const raytracer::object::scene::IScene &scene,
                       const std::vector<maths::Color> &pixels) const override;

      private:
        static void writeColor(const maths::Color color);
    };

}  // namespace raytracer::exporter
