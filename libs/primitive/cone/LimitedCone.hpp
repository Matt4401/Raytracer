/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LimitedCone
*/

#pragma once

#include <any>
#include <map>

#include "Cone.hpp"
#include "math/Ray.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class LimitedCone : public Cone {
      public:
        using object::primitive::IPrimitive::hits;

        explicit LimitedCone(const std::map<std::string, std::any> &args);
        explicit LimitedCone(const maths::Vector &center, double openingFactor,
                             double height, bool closed = false);
        ~LimitedCone() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      protected:
        bool validateConstraints(const maths::Vector &p) const override;

      private:
        bool testCapIntersection(const maths::Ray &ray, double yCap,
                                 HitRecord &hitRecord) const;

        double _height;
        bool _closed = false;
    };
}  // namespace raytracer::object::primitive
