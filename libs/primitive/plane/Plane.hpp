/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
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
    class Plane : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Plane(const std::map<std::string, std::any> &args);
        explicit Plane(const maths::Vector &vector,
                       const maths::Vector &normal);
        explicit Plane(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &vector, const maths::Vector &normal);

        ~Plane() override = default;

        const maths::Vector &normal() const noexcept;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        maths::Vector _normal;

        static constexpr std::size_t EXPECTED_ARGS = 3;
        static constexpr double EPS = K_RAY_EPSILON;
    };
}  // namespace raytracer::object::primitive