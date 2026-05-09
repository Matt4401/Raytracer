/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
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
    class Triangle : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Triangle(const std::map<std::string, std::any> &args);
        explicit Triangle(const maths::Vector &center, const maths::Vector &v1,
                          const maths::Vector &v2);
        explicit Triangle(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, const maths::Vector &v1,
            const maths::Vector &v2);

        ~Triangle() override = default;

        maths::Vector v1() const noexcept;
        maths::Vector v2() const noexcept;

        double hits(const maths::Ray &ray) override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        maths::Vector _v1;
        maths::Vector _v2;

        static constexpr std::size_t EXPECTED_ARGS = 4;
    };
}  // namespace raytracer::object::primitive
