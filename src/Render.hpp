/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Render
*/

#ifndef RENDER_HPP_
#define RENDER_HPP_

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/ICamera.hpp"
#include "object/IScene.hpp"

namespace raytracer {
    class Render {
      public:
        Render() = default;
        ~Render() = default;

        /// @brief Render the scene and populate the internal pixel buffer.
        /// @param scene The scene to render.
        /// @param pixel Pixel parameter (unused).
        /// @param samples Number of samples per pixel.
        void render(const object::scene::IScene &scene, int pixel = 1,
                    int samples = 40);

        const std::vector<maths::Color> &pixels() const;

      protected:
      private:
        std::thread printProgress(int activeWorkers, int imageHeight);
        void renderRows(const object::scene::IScene &scene,
                        unsigned int workerId, int imageHeight);

        struct RenderState {
            const object::scene::IScene *scene = nullptr;
            const raytracer::object::camera::ICamera *cam = nullptr;
            double invImageWidth = 0.0;
            double invImageHeight = 0.0;
            maths::Vector cx{0, 0, 0};
            maths::Vector cy{0, 0, 0};
            double sampleWeight = 0.0;
            unsigned short *xi = nullptr;
        };

        static constexpr double KCX_FACTOR = 0.5135;

        /// @brief Compute stratified jitter offsets for a sample.
        static void computeStratifiedSample(unsigned short *xi, double &dx,
                                            double &dy);

        /// @brief Cast a primary ray for a pixel and subpixel.
        static maths::Ray castPrimaryRay(const RenderState &st, int x, int y,
                                         int sx, int sy, double dx, double dy);

        /// @brief Sample a subpixel and accumulate radiance.
        maths::Vector sampleSubpixel(const RenderState &st, int x, int y,
                                     int sx, int sy) const;

        /// @brief Compute final pixel color from all subpixels.
        maths::Color computePixelColor(const RenderState &st, int x,
                                       int y) const;

        void initRender(const object::scene::IScene &scene, int samples,
                        int &imageWidth, int &imageHeight,
                        unsigned int &workerCount);
        void startWorkers(const object::scene::IScene &scene,
                          unsigned int workerCount,
                          unsigned int &activeWorkers);
        void finishRender(
            unsigned int activeWorkers, int imageHeight,
            const std::chrono::high_resolution_clock::time_point &startTotal);

        std::vector<maths::Color> _pixels;

        std::vector<std::thread> _workers;
        std::unique_ptr<std::atomic<int>[]> _workerDone;
        std::vector<int> _workerRows;
        std::atomic<bool> _renderingFinished;
        std::atomic<int> _nextRow{0};
        int _samples = 40;
    };
}  // namespace raytracer

#endif /* !RENDER_HPP_ */
