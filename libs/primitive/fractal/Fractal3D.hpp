/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Fractal3D
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "fractal/IFractalStrategy.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Fractal3D : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        static constexpr double DEFAULT_EPS = 1e-5;
        static constexpr int DEFAULT_MAX_STEPS = 200;
        static constexpr double DEFAULT_MAX_DIST = 100.0;

        explicit Fractal3D(const std::map<std::string, std::any> &args);
        explicit Fractal3D(std::shared_ptr<fractal::IFractalStrategy> strategy,
                           const maths::Vector &center,
                           int maxSteps = DEFAULT_MAX_STEPS,
                           double epsilon = DEFAULT_EPS,
                           double maxDist = DEFAULT_MAX_DIST);
        explicit Fractal3D(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            std::shared_ptr<fractal::IFractalStrategy> strategy,
            const maths::Vector &center, int maxSteps = DEFAULT_MAX_STEPS,
            double epsilon = DEFAULT_EPS, double maxDist = DEFAULT_MAX_DIST);

        ~Fractal3D() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      private:
        [[nodiscard]] maths::Vector computeNormal(
            const maths::Vector &localPoint) const noexcept;

        std::shared_ptr<fractal::IFractalStrategy> _strategy;
        int _maxSteps;
        double _epsilon;
        double _maxDist;

        static constexpr double NORMAL_H = 1e-4;
        static constexpr std::size_t EXPECTED_ARGS = 5;
    };

}  // namespace raytracer::object::primitive