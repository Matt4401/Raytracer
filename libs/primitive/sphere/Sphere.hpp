/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::primitive {
    class Sphere : public APrimitive {
      public:
        explicit Sphere(const std::map<std::string, std::any> &params);
        explicit Sphere(const maths::Vector &vector,
                        const maths::Vector &emission,
                        const maths::Color &color, double radius, RefltT refl);
        ~Sphere() override = default;

        const double &radius() const noexcept;

        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;

      private:
        double _radius;

        static constexpr std::size_t EXPECTED_ARGS = 5;
        static constexpr double EPS = 1e-4;
    };
}  // namespace raytracer::object::primitive
