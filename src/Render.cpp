/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Render
*/

#include "Render.hpp"

#include <chrono>
#include <iostream>

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
        _samples = samples;
        int imageWidth = scene.cameras().at(0)->imageWidth();
        int imageHeight = scene.cameras().at(0)->imageHeight();
        _pixels.assign(imageWidth * imageHeight, maths::Color());

        const unsigned int workerCount =
            std::max(1u, std::thread::hardware_concurrency());
        const int rowsPerWorker = (imageHeight + workerCount - 1) / workerCount;

        this->_workers.clear();
        _workerDone = std::make_unique<std::atomic<int>[]>(workerCount);
        _workerRows = std::vector<int>(workerCount);
        _renderingFinished = false;
        unsigned int activeWorkers = 0;

        std::atomic<int> *workerDone = _workerDone.get();
        for (unsigned int i = 0; i < workerCount; ++i) workerDone[i].store(0);
        _workers.reserve(workerCount);

        for (unsigned int w = 0; w < workerCount; ++w) {
            int yStart = w * rowsPerWorker;
            int yEnd = std::min(imageHeight, yStart + rowsPerWorker);
            if (yStart >= imageHeight)
                break;
            _workerRows[w] = yEnd - yStart;
            _workers.emplace_back([this, &scene, w, yStart, yEnd]() {
                renderRows(scene, w, yStart, yEnd);
            });
            ++activeWorkers;
        }

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

        const maths::Vector cx((imageWidth * 0.5135) / imageHeight, 0, 0);
        const maths::Vector cy =
            cx.cross(scene.cameras().at(0)->rotation()).normalized() * 0.5135;
        for (int y = yStart; y < yEnd; ++y) {
            unsigned short Xi[3] = {
                0, 0, (unsigned short)((y + 1) * (y + 1) * (y + 1))};
            for (int x = 0; x < imageWidth; ++x) {
                const int idx = (imageHeight - y - 1) * imageWidth + x;
                maths::Vector pixel(0, 0, 0);

                for (int sy = 0; sy < 2; sy++) {
                    for (int sx = 0; sx < 2; sx++) {
                        maths::Vector r(0, 0, 0);
                        for (int s = 0; s < samplesPerSubpixel; s++) {
                            double r1 = 2 * ::erand48(Xi);
                            double dx = r1 < 1 ? std::sqrt(r1) - 1
                                               : 1 - std::sqrt(2 - r1);
                            double r2 = 2 * ::erand48(Xi);
                            double dy = r2 < 1 ? std::sqrt(r2) - 1
                                               : 1 - std::sqrt(2 - r2);

                            double px =
                                ((sx + 0.5 + dx) / 2.0 + x) * invImageWidth -
                                0.5;
                            double py =
                                ((sy + 0.5 + dy) / 2.0 + y) * invImageHeight -
                                0.5;
                            maths::Vector d = cx * px + cy * py +
                                              scene.cameras().at(0)->rotation();
                            maths::Ray primary(
                                scene.cameras().at(0)->position() + d * 140.0,
                                d.normalized());

                            r = r +
                                scene.radiance(primary, 0, Xi) * sampleWeight;
                        }
                        pixel =
                            pixel + maths::Vector(std::clamp(r.x, 0.0, 1.0),
                                                  std::clamp(r.y, 0.0, 1.0),
                                                  std::clamp(r.z, 0.0, 1.0)) *
                                        0.25;
                    }
                }
                _pixels[idx] =
                    maths::Color(std::clamp(pixel.x * 255.0, 0.0, 255.0),
                                 std::clamp(pixel.y * 255.0, 0.0, 255.0),
                                 std::clamp(pixel.z * 255.0, 0.0, 255.0));
            }
            std::atomic<int> *workerDone = _workerDone.get();
            workerDone[workerId].fetch_add(1);
        }
    }
}  // namespace raytracer