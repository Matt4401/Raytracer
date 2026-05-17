/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Torus
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Torus : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Torus(const std::map<std::string, std::any> &args);
        explicit Torus(const maths::Vector &center, double majorRadius,
                       double minorRadius);
        explicit Torus(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, double majorRadius,
            double minorRadius);

        ~Torus() override = default;

        const double &majorRadius() const noexcept;
        const double &minorRadius() const noexcept;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      private:
        static maths::Vector computeNormal(const maths::Vector &localPoint,
                                           double upperR);

        double _majorRadius;
        double _minorRadius;

        static constexpr std::size_t EXPECTED_ARGS = 4;
        static constexpr double EPSILON_AXIS = 1e-10;
        static constexpr double MIN_RAY_DIRECTION_MAGNITUDE = 1e-9;
        static constexpr double NEWTON_DERIVATIVE_EPSILON = 1e-15;
    };
}  // namespace raytracer::object::primitive