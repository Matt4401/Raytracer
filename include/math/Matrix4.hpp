/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Matrix4
*/

#pragma once

#include <array>
#include <cstddef>

#include "Vector.hpp"

namespace raytracer::maths {
    class Matrix4 {
      public:
        Matrix4();
        explicit Matrix4(double diagonal);

        static Matrix4 identity();
        static Matrix4 zero();
        static Matrix4 translation(const Vector &translation);
        static Matrix4 scale(const Vector &scale);
        static Matrix4 shear(double xy, double xz, double yx, double yz,
                             double zx, double zy);
        static Matrix4 rotationX(double angle);
        static Matrix4 rotationY(double angle);
        static Matrix4 rotationZ(double angle);

        double at(std::size_t row, std::size_t column) const;
        void set(std::size_t row, std::size_t column, double value);

        Matrix4 operator*(const Matrix4 &other) const;
        Matrix4 &operator*=(const Matrix4 &other);

        Vector transformPoint(const Vector &point) const;
        Vector transformVector(const Vector &vector) const;
        Vector transformNormal(const Vector &normal) const;

        Matrix4 transpose() const;
        Matrix4 inverse() const;

      private:
        std::array<std::array<double, 4>, 4> _values;

        void setIdentity();
    };
}  // namespace raytracer::maths