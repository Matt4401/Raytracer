/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#pragma once

#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Triangle : public APrimitive {
      public:
        explicit Triangle(const std::map<std::string, std::any> &args);
        ~Triangle() override = default;

        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      private:
        maths::Vector _v0;
        maths::Vector _v1;
        maths::Vector _v2;

        static constexpr std::size_t EXPECTED_ARGS = 4;
        static constexpr double EPS = kRayEpsilon;
    };
}  // namespace raytracer::object::primitive