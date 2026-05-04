/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Rectangle
*/

#include "math/Rectangle.hpp"

namespace raytracer::maths {
    Rectangle::Rectangle(const Vector &origin, const Vector &bottomSide,
                         const Vector &leftSide)
        : _origin(origin), _bottomSide(bottomSide), _leftSide(leftSide) {
    }

    Rectangle::Rectangle() : _origin(), _bottomSide(), _leftSide() {
    }

    Vector Rectangle::pointAt(const double u, const double v) const {
        return Vector(_origin.x + u * _bottomSide.x + v * _leftSide.x,
                      _origin.y + u * _bottomSide.y + v * _leftSide.y,
                      _origin.z + u * _bottomSide.z + v * _leftSide.z);
    }
}  // namespace raytracer::maths