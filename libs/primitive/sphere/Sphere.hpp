/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
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
    class Sphere : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Sphere(const std::map<std::string, std::any> &args);
        explicit Sphere(const maths::Vector &vector, double radius);
        explicit Sphere(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &vector, double radius);

        ~Sphere() override = default;

        const double &radius() const noexcept;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        double _radius;

        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}  // namespace raytracer::object::primitive
