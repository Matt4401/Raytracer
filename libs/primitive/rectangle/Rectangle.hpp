/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Rectangle
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
    class Rectangle : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Rectangle(const std::map<std::string, std::any> &args);
        explicit Rectangle(const maths::Vector &center,
                   const maths::Vector &maxPoint,
                   const maths::Vector &normal);
        explicit Rectangle(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
          const maths::Vector &center, const maths::Vector &maxPoint,
          const maths::Vector &normal);

        ~Rectangle() override = default;

        maths::Vector maxPoint() const;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        maths::Vector _maxPoint;
        maths::Vector _normal;

        static constexpr std::size_t EXPECTED_ARGS = 4;
        static constexpr double EPS = K_RAY_EPSILON;
    };
}  // namespace raytracer::object::primitive
