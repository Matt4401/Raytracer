/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LimitedCylinder
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "Cylinder.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    class LimitedCylinder : public Cylinder {
      public:
        using object::primitive::IPrimitive::hits;

        explicit LimitedCylinder(const std::map<std::string, std::any> &args);
        explicit LimitedCylinder(const maths::Vector &center, double radius,
                                 double height, bool closed = false);
        ~LimitedCylinder() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      protected:
        bool validateConstraints(const maths::Vector &p) const override;

      private:
        double _height;
        bool _closed = false;
    };
}  // namespace raytracer::object::primitive
