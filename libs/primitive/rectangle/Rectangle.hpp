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
                           const maths::Vector &maxPoint);
        explicit Rectangle(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, const maths::Vector &maxPoint);

        ~Rectangle() override = default;

        maths::Vector maxPoint() const;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        maths::Vector _maxPoint;

        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}  // namespace raytracer::object::primitive
