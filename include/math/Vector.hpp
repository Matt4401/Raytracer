
/*
** EPITECH PROJECT, 2026
** Vector
** File description:
** Vector header
*/

#pragma once

#include <cmath>
namespace raytracer {

    class Vector {
      public:
        Vector() = default;
        explicit Vector(double x, double y, double z) : x(x), y(y), z(z) {};
        ~Vector() = default;

        double length() {
            double tot = 0;

            return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2) +
                             std::pow(this->z, 2));
        };

        double dot(const Vector& other) const {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }

        double x = 0;
        double y = 0;
        double z = 0;
    };

}  // namespace raytracer
