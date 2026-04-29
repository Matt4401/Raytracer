/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#pragma once

#include <any>
#include <vector>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Sphere : public APrimitive {
      public:
        explicit Sphere(const std::vector<std::any> &args);
        explicit Sphere(const maths::Vector &vector, double radius);
        ~Sphere() override = default;

        const double &radius() const noexcept;

        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector& hitPoint) const override;
        Type type() const override { return Type::PRIMITIVE; }

      private:
        double _radius;

        static constexpr std::size_t EXPECTED_ARGS = 2;
        static constexpr double EPS = 1e-4;
    };
}  // namespace raytracer::object::primitive
