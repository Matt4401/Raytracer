/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BoundingBox
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

#include "math/Ray.hpp"

namespace raytracer::maths {
    class BoundingBox {
      public:
        double x = 0;
        double y = 0;
        double z = 0;
        double w = 0;
        double h = 0;
        double d = 0;

        BoundingBox();

        void extend(const BoundingBox &other);
        double surfaceData() const;
        bool intersects(const Ray &ray) const;
    };

}  // namespace raytracer::maths
