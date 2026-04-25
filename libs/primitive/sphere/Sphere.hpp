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
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::primitive {
    class Sphere : public APrimitive {
      public:
        explicit Sphere(const std::vector<std::any> &args);
        explicit Sphere(const maths::Vector &vector,
                        const maths::Vector &emission,
                        const maths::Vector &color, double radius, RefltT refl);
        ~Sphere() override = default;

        const double &radius() const noexcept;

        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;

      private:
        double _radius;

        static constexpr std::size_t EXPECTED_ARGS = 5;
    };
}  // namespace raytracer::object::primitive
