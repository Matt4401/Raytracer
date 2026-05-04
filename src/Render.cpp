/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Render
*/

#include "Render.hpp"

#include <chrono>
#include <iostream>
#include <stdlib.h>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/ICamera.hpp"

namespace raytracer {

    static void writeColor(maths::Color color) {
        std::cout << static_cast<int>(color.r) << " "
                  << static_cast<int>(color.g) << " "
                  << static_cast<int>(color.b) << "\n";
    }

    void Render::computeStratifiedSample(unsigned short *Xi, double &dx,
                                         double &dy) const {
        double r1 = 2 * ::erand48(Xi);
        dx = r1 < 1 ? std::sqrt(r1) - 1 : 1 - std::sqrt(2 - r1);
        double r2 = 2 * ::erand48(Xi);
        dy = r2 < 1 ? std::sqrt(r2) - 1 : 1 - std::sqrt(2 - r2);
    }

    maths::Ray Render::castPrimaryRay(const Render::RenderState &st, int x,
                                      int y, int sx, int sy, double dx,
                                      double dy) const {
        double px = ((sx + 0.5 + dx) / 2.0 + x) * st.invImageWidth - 0.5;
        double py = ((sy + 0.5 + dy) / 2.0 + y) * st.invImageHeight - 0.5;
        maths::Vector d = st.cx * px + st.cy * py + st.cam->rotation();
        return maths::Ray(st.cam->position() + d * 140.0, d.normalized());
    }

    maths::Vector Render::sampleSubpixel(const Render::RenderState &st, int x,
                                         int y, int sx, int sy) const {
        maths::Vector radiance(0, 0, 0);
        const int samplesPerSubpixel = std::max(1, _samples / 4);

        for (int s = 0; s < samplesPerSubpixel; ++s) {
            double dx, dy;
            computeStratifiedSample(st.Xi, dx, dy);
            maths::Ray primary = castPrimaryRay(st, x, y, sx, sy, dx, dy);
            radiance = radiance +
                       st.scene->radiance(primary, 0, st.Xi) * st.sampleWeight;
        }
        return radiance;
    }

    maths::Color Render::computePixelColor(const Render::RenderState &st, int x,
                                           int y) const {
        maths::Vector pixel(0, 0, 0);

        for (int sy = 0; sy < 2; ++sy) {
            for (int sx = 0; sx < 2; ++sx) {
                maths::Vector subpixel = sampleSubpixel(st, x, y, sx, sy);
                pixel =
                    pixel + maths::Vector(std::clamp(subpixel.x, 0.0, 1.0),
                                          std::clamp(subpixel.y, 0.0, 1.0),
                                          std::clamp(subpixel.z, 0.0, 1.0)) *
                                0.25;
            }
        }

        return maths::Color(std::clamp(pixel.x * 255.0, 0.0, 255.0),
                            std::clamp(pixel.y * 255.0, 0.0, 255.0),
                            std::clamp(pixel.z * 255.0, 0.0, 255.0));
    }

    std::thread Render::printProgress(int activeWorkers, int imageHeight) {
        return std::thread([this, activeWorkers, imageHeight]() {
            while (!_renderingFinished.load()) {
                int done = 0;
                for (unsigned int w = 0; w < activeWorkers; ++w)
                    done += _workerDone[w].load();
                double pct =
                    imageHeight == 0 ? 100.0 : (100.0 * done) / imageHeight;
                std::cerr << "\rRender " << (int)pct << "% ";
                for (unsigned int w = 0; w < activeWorkers; ++w) {
                    int rows = _workerRows[w] == 0 ? 1 : _workerRows[w];
                    std::cerr << "| W" << w << ":"
                              << (int)(100.0 * _workerDone[w].load() / rows)
                              << "% ";
                }
                std::cerr << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            std::cerr << "\rRender 100%\n";
        });
    }

    void Render::render(const object::scene::IScene &scene, int pixel,
                        int samples) {
        auto startTotal = std::chrono::high_resolution_clock::now();
        int imageWidth = 0;
        int imageHeight = 0;
        unsigned int workerCount = 0;
        int rowsPerWorker = 0;

        initRender(scene, samples, imageWidth, imageHeight, workerCount,
                   rowsPerWorker);

        unsigned int activeWorkers = 0;
        startWorkers(scene, workerCount, rowsPerWorker, activeWorkers);

        finishRender(activeWorkers, imageHeight, startTotal);
    }

    void Render::initRender(const object::scene::IScene &scene, int samples,
                            int &imageWidth, int &imageHeight,
                            unsigned int &workerCount, int &rowsPerWorker) {
        _samples = samples;
        imageWidth = scene.cameras().at(0)->imageWidth();
        imageHeight = scene.cameras().at(0)->imageHeight();
        _pixels.assign(imageWidth * imageHeight, maths::Color());

        workerCount = std::max(1u, std::thread::hardware_concurrency());
        rowsPerWorker = (imageHeight + workerCount - 1) / workerCount;

        this->_workers.clear();
        _workerDone = std::make_unique<std::atomic<int>[]>(workerCount);
        _workerRows = std::vector<int>(workerCount);
        _renderingFinished = false;

        std::atomic<int> *workerDone = _workerDone.get();
        for (unsigned int i = 0; i < workerCount; ++i) workerDone[i].store(0);
        _workers.reserve(workerCount);
    }

    void Render::startWorkers(const object::scene::IScene &scene,
                              unsigned int workerCount, int rowsPerWorker,
                              unsigned int &activeWorkers) {
        for (unsigned int w = 0; w < workerCount; ++w) {
            int yStart = w * rowsPerWorker;
            int yEnd = std::min(scene.cameras().at(0)->imageHeight(),
                                yStart + rowsPerWorker);
            if (yStart >= scene.cameras().at(0)->imageHeight())
                break;
            _workerRows[w] = yEnd - yStart;
            _workers.emplace_back([this, &scene, w, yStart, yEnd]() {
                renderRows(scene, w, yStart, yEnd);
            });
            ++activeWorkers;
        }
    }

    void Render::finishRender(
        unsigned int activeWorkers, int imageHeight,
        const std::chrono::high_resolution_clock::time_point &startTotal) {
        auto progressThread = printProgress(activeWorkers, imageHeight);

        for (auto &w : _workers) w.join();
        _renderingFinished.store(true);
        progressThread.join();

        auto endRender = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      endRender - startTotal)
                      .count();
        std::cerr << "Render time: " << ms / 60000 << "min "
                  << (ms % 60000) / 1000 << "s\n";
    }

    void Render::pixelToPPM(const object::scene::IScene &scene) const {
        int imageWidth = scene.cameras().at(0)->imageWidth();
        int imageHeight = scene.cameras().at(0)->imageHeight();
        std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
        for (const auto &col : _pixels) writeColor(col);
    }

    void Render::pixelToPPM(const object::scene::IScene &scene,
                            const std::vector<maths::Color> &pixels) const {
        int imageWidth = scene.cameras().at(0)->imageWidth();
        int imageHeight = scene.cameras().at(0)->imageHeight();
        std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
        for (const auto &col : pixels) writeColor(col);
    }

    void Render::renderRows(const object::scene::IScene &scene,
                            unsigned int workerId, int yStart, int yEnd) {
        int imageWidth = scene.cameras().at(0)->imageWidth();
        int imageHeight = scene.cameras().at(0)->imageHeight();
        const double invImageWidth = 1.0 / imageWidth;
        const double invImageHeight = 1.0 / imageHeight;

        const int samplesPerSubpixel = std::max(1, _samples / 4);
        const double sampleWeight = 1.0 / samplesPerSubpixel;

        const maths::Vector cx((imageWidth * Render::kCxFactor) / imageHeight,
                               0, 0);
        const maths::Vector cy =
            cx.cross(scene.cameras().at(0)->rotation()).normalized() *
            Render::kCxFactor;

        Render::RenderState st;
        st.scene = &scene;
        st.cam = scene.cameras().at(0).get();
        st.invImageWidth = invImageWidth;
        st.invImageHeight = invImageHeight;
        st.cx = cx;
        st.cy = cy;
        st.sampleWeight = sampleWeight;

        for (int y = yStart; y < yEnd; ++y) {
            unsigned short Xi[3] = {
                0, 0, (unsigned short)((y + 1) * (y + 1) * (y + 1))};
            st.Xi = Xi;
            for (int x = 0; x < imageWidth; ++x) {
                const int idx = (imageHeight - y - 1) * imageWidth + x;
                _pixels[idx] = computePixelColor(st, x, y);
            }
            std::atomic<int> *workerDone = _workerDone.get();
            workerDone[workerId].fetch_add(1);
        }
    }
}  // namespace raytracer