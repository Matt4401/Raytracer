/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Render
*/

#ifndef RENDER_HPP_
#define RENDER_HPP_

#include <algorithm>
#include <atomic>
#include <cmath>
#include <fstream>
#include <memory>
#include <thread>

#include "object/IScene.hpp"

namespace raytracer {
    class Render {
      public:
        Render() = default;
        ~Render() = default;

        std::thread printProgress(int activeWorkers, int imageHeight);

        /// @brief Render the scene and populate the internal pixel buffer.
        /// @param scene The scene to render.
        /// @param pixel Pixel parameter (unused).
        /// @param samples Number of samples per pixel.
        void render(const object::scene::IScene &scene, int pixel = 1,
                    int samples = 40);

        /// @brief Save the rendered image to a file in PPM format.
        void pixelToPPM(const object::scene::IScene &scene) const;
        /// @brief Save the rendered image to a file in PPM format.
        /// @param pixels The 2D vector of Color objects representing the
        /// rendered image.
        void pixelToPPM(const object::scene::IScene &scene,
                        const std::vector<maths::Color> &pixels) const;

      protected:
      private:
        void renderRows(const object::scene::IScene &scene,
                        unsigned int workerId, int yStart, int yEnd);
        std::vector<maths::Color> _pixels;

        std::vector<std::thread> _workers;
        std::unique_ptr<std::atomic<int>[]> _workerDone;
        std::vector<int> _workerRows;
        std::atomic<bool> _renderingFinished;
        int _samples = 40;
    };
}  // namespace raytracer

#endif /* !RENDER_HPP_ */
