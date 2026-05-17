/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Render
*/

#include "Render.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/ICamera.hpp"

namespace raytracer::render_detail {
    static constexpr double K_CAMERA_FACTOR = 0.5135;

    static maths::Vector pickReferenceAxis(const maths::Vector &forward) {
        const maths::Vector worldUp(0, 1, 0);
        if (std::fabs(forward.dot(worldUp)) > 0.999) {
            return maths::Vector(0, 0, 1);
        }
        return worldUp;
    }

    static void buildCameraBasis(const maths::Vector &rotation,
                                 const double aspectRatio, maths::Vector &cx,
                                 maths::Vector &cy, maths::Vector &forward) {
        forward = rotation;
        if (forward.magnitude() == 0.0) {
            forward = maths::Vector(0, 0, -1);
        }
        forward = forward.normalized();

        const maths::Vector reference = pickReferenceAxis(forward);
        maths::Vector right = forward.cross(reference);
        if (right.magnitude() == 0.0) {
            right = forward.cross(maths::Vector(1, 0, 0));
        }
        if (right.magnitude() == 0.0) {
            right = maths::Vector(1, 0, 0);
        } else {
            right = right.normalized();
        }

        maths::Vector up = right.cross(forward);
        if (up.magnitude() == 0.0) {
            up = maths::Vector(0, 1, 0);
        } else {
            up = up.normalized();
        }

        cx = right * (aspectRatio * K_CAMERA_FACTOR);
        cy = up * K_CAMERA_FACTOR;
    }
}  // namespace raytracer::render_detail

namespace raytracer {

    void Render::computeStratifiedSample(unsigned short *xi, double &dx,
                                         double &dy) {
        double r1 = 2 * ::erand48(xi);
        dx = r1 < 1 ? std::sqrt(r1) - 1 : 1 - std::sqrt(2 - r1);
        double r2 = 2 * ::erand48(xi);
        dy = r2 < 1 ? std::sqrt(r2) - 1 : 1 - std::sqrt(2 - r2);
    }

    maths::Ray Render::castPrimaryRay(const Render::RenderState &st, int x,
                                      int y, int sx, int sy, double dx,
                                      double dy) {
        double px = ((sx + 0.5 + dx) / 2.0 + x) * st.invImageWidth - 0.5;
        double py = ((sy + 0.5 + dy) / 2.0 + y) * st.invImageHeight - 0.5;
        maths::Vector d = st.cx * px + st.cy * py + st.forward;
        return maths::Ray(st.cam->position() + d * 140.0, d.normalized());
    }

    maths::Vector Render::sampleSubpixel(const Render::RenderState &st, int x,
                                         int y, int sx, int sy) const {
        maths::Vector radiance(0, 0, 0);
        const int samplesPerSubpixel = std::max(1, _samples / 4);

        for (int s = 0; s < samplesPerSubpixel; ++s) {
            double dx, dy;
            computeStratifiedSample(st.xi, dx, dy);
            maths::Ray primary = castPrimaryRay(st, x, y, sx, sy, dx, dy);
            radiance = radiance +
                       st.scene->radiance(primary, 0, st.xi) * st.sampleWeight;
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

    bool Render::renderingIsFinished() const {
        return _renderingFinished.load();
    }

    bool Render::renderedStopped() const {
        return this->_stopRendering.load();
    }

    int Render::getPercentRendered(int activeWorkers) const {
        int done = 0;

        for (int w = 0; w < activeWorkers; ++w) {
            done += _workerDone[w].load();
        }
        return (int)this->_imageSize.heigth == 0
                   ? 100.0
                   : (100.0 * done) / this->_imageSize.heigth;
    }

    ImageSize Render::imageSize() {
        return this->_imageSize;
    }

    void Render::setPrintProgressCallback(
        const PrintProgressCallback &callback) {
        this->_printCallback = callback;
    }

    void Render::stopRendering() {
        this->_stopRendering.store(true);
    }

    void Render::render(const object::scene::IScene &scene, int samples) {
        auto startTotal = std::chrono::high_resolution_clock::now();
        unsigned int workerCount = 0;

        initRender(scene, samples, workerCount);

        unsigned int activeWorkers = 0;
        startWorkers(scene, workerCount, activeWorkers);

        finishRender(activeWorkers, startTotal);
    }

    void Render::initRender(const object::scene::IScene &scene, int samples,
                            unsigned int &workerCount) {
        const auto &camera = scene.cameras().at(0);

        this->_pixels.clear();
        this->_workers.clear();
        this->_pixels.clear();
        this->_renderingFinished.store(false);
        this->_nextRow.store(0);

        _samples = samples;
        this->_imageSize.width = camera->imageWidth();
        this->_imageSize.heigth = camera->imageHeight();
        _pixels.assign(this->_imageSize.width * this->_imageSize.heigth,
                       maths::Color());

        workerCount = std::max(1u, std::thread::hardware_concurrency());

        this->_workers.clear();
        _workerDone = std::make_unique<std::atomic<int>[]>(workerCount);
        _workerRows = std::vector<int>(workerCount);
        _renderingFinished = false;
        _stopRendering.store(false);
        _nextRow.store(0);

        std::atomic<int> *workerDone = _workerDone.get();
        for (unsigned int i = 0; i < workerCount; ++i) workerDone[i].store(0);
        _workers.reserve(workerCount);
    }

    void Render::startWorkers(const object::scene::IScene &scene,
                              unsigned int workerCount,
                              unsigned int &activeWorkers) {
        const int imageHeight = scene.cameras().at(0)->imageHeight();
        for (unsigned int w = 0; w < workerCount; ++w) {
            _workerRows[w] = imageHeight;
            _workers.emplace_back([this, &scene, w, imageHeight]() {
                renderRows(scene, w, imageHeight);
            });
            ++activeWorkers;
        }
    }

    void Render::finishRender(
        unsigned int activeWorkers,
        const std::chrono::high_resolution_clock::time_point &startTotal) {
        auto progressThread = this->_printCallback(activeWorkers, *this);

        for (auto &w : _workers) w.join();
        _renderingFinished.store(true);
        this->_workers.clear();
        progressThread.join();

        auto endRender = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      endRender - startTotal)
                      .count();
        if (!this->_stopRendering.load()) {
            std::cerr << "Render time: " << ms / 60000 << "min "
                      << (ms % 60000) / 1000 << "s\n";
        }
    }

    std::vector<maths::Color> &Render::pixels() {
        return this->_pixels;
    }

    void Render::renderRows(const object::scene::IScene &scene,
                            unsigned int workerId, int imageHeight) {
        const auto &camera = scene.cameras().at(0);
        int imageWidth = camera->imageWidth();
        const double invImageWidth = 1.0 / imageWidth;
        const double invImageHeight = 1.0 / imageHeight;

        const int samplesPerSubpixel = std::max(1, _samples / 4);
        const double sampleWeight = 1.0 / samplesPerSubpixel;

        maths::Vector cx;
        maths::Vector cy;
        maths::Vector forward;
        render_detail::buildCameraBasis(
            camera->rotation(),
            static_cast<double>(imageWidth) / static_cast<double>(imageHeight),
            cx, cy, forward);

        Render::RenderState st;
        st.scene = &scene;
        st.cam = camera.get();
        st.invImageWidth = invImageWidth;
        st.invImageHeight = invImageHeight;
        st.cx = cx;
        st.cy = cy;
        st.forward = forward;
        st.sampleWeight = sampleWeight;

        while (!(this->_stopRendering.load())) {
            const int y = _nextRow.fetch_add(1);
            if (y >= imageHeight) {
                break;
            }
            unsigned short xi[3] = {
                0, 0, static_cast<unsigned short>((y + 1) * (y + 1) * (y + 1))};
            st.xi = xi;
            for (int x = 0; x < imageWidth; ++x) {
                if (this->_stopRendering.load()) {
                    break;
                }
                const int idx = (imageHeight - y - 1) * imageWidth + x;
                _pixels[idx] = computePixelColor(st, x, y);
            }
            std::atomic<int> *workerDone = _workerDone.get();
            workerDone[workerId].fetch_add(1);
        }
    }
}  // namespace raytracer
