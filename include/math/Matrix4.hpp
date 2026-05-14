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

        /**
         * @brief Creates an identity matrix.
         * @return An identity matrix.
         */
        static Matrix4 identity();
        /**
         * @brief Creates a zero matrix.
         * @return A zero matrix.
         */
        static Matrix4 zero();
        /**
         * @brief Creates a translation matrix.
         * @param translation The translation vector.
         * @return A translation matrix.
         */
        static Matrix4 translation(const Vector &translation);
        /**
         * @brief Creates a scale matrix.
         * @param scale The scale vector.
         * @return A scale matrix.
         */
        static Matrix4 scale(const Vector &scale);
        /**
         * @brief Creates a shear matrix.
         * @param xy The shear factor for x and y.
         * @param xz The shear factor for x and z.
         * @param yx The shear factor for y and x.
         * @param yz The shear factor for y and z.
         * @param zx The shear factor for z and x.
         * @param zy The shear factor for z and y.
         * @return A shear matrix.
         */
        static Matrix4 shear(double xy, double xz, double yx, double yz,
                               double zx, double zy);
        /**
         * @brief Creates a rotation matrix around the X-axis.
         * @param angle The rotation angle in radians.
         * @return A rotation matrix around the X-axis.
         */
        static Matrix4 rotationX(double angle);
        /**
         * @brief Creates a rotation matrix around the Y-axis.
         * @param angle The rotation angle in radians.
         * @return A rotation matrix around the Y-axis.
         */
        static Matrix4 rotationY(double angle);
        /**
         * @brief Creates a rotation matrix around the Z-axis.
         * @param angle The rotation angle in radians.
         * @return A rotation matrix around the Z-axis.
         */
        static Matrix4 rotationZ(double angle);

        /**
         * @brief Accesses the element at the specified row and column.
         * @param row The row index (0-based).
         * @param column The column index (0-based).
         * @return double The value at the specified row and column.
         * @throws std::out_of_range if the row or column index is out of bounds
         */
        double at(std::size_t row, std::size_t column) const;
        /**
         * @brief Sets the element at the specified row and column to the given value.
         * @param row The row index (0-based).
         * @param column The column index (0-based).
         * @param value The value to set at the specified row and column.
          * @throws std::out_of_range if the row or column index is out of bounds
         */
        void set(std::size_t row, std::size_t column, double value);

        /**
         * @brief Multiplies this matrix by another matrix and returns the result.
         * @param other The matrix to multiply with this matrix.
         * @return Matrix4 The result of multiplying this matrix by the other matrix.
         */
        Matrix4 operator*(const Matrix4 &other) const;
        Matrix4 &operator*=(const Matrix4 &other);

        /**
         * @brief Transforms a point using this matrix. The point is treated as a homogeneous coordinate with w = 1.
         * @param point The point to transform.
         * @return Vector The transformed point.
         */
        Vector transformPoint(const Vector &point) const;
        /**
         * @brief Transforms a vector using this matrix. The vector is treated as a homogeneous coordinate with w = 0.
         * @param vector The vector to transform.
         * @return Vector The transformed vector.
         */
        Vector transformVector(const Vector &vector) const;
        /**
         * @brief Transforms a normal vector using this matrix. The normal is transformed using the inverse transpose of the matrix to ensure correct transformation under non-uniform scaling.
         * @param normal The normal vector to transform.
         * @return Vector The transformed normal vector.
         */
        Vector transformNormal(const Vector &normal) const;

        /**
         * @brief Transposes this matrix and returns the result.
         * @return Matrix4 The transposed matrix.
         */
        Matrix4 transpose() const;
        /**
         * @brief Computes the inverse of this matrix and returns the result.
         * @return Matrix4 The inverse matrix.
         */
        Matrix4 inverse() const;

      private:
        std::array<std::array<double, 4>, 4> _values;

        /**
         * @brief Set the Identity object to this matrix.
         */
        void setIdentity();
    };
}  // namespace raytracer::maths