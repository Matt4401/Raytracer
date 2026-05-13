/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Matrix4
*/

#include "math/Matrix4.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace raytracer::maths {
    namespace {
        constexpr double EPSILON = 1e-12;
    }

    Matrix4::Matrix4() {
        setIdentity();
    }

    Matrix4::Matrix4(double diagonal) {
        _values = {};
        for (std::size_t index = 0; index < 4; ++index) {
            _values[index][index] = diagonal;
        }
    }

    Matrix4 Matrix4::identity() {
        return Matrix4();
    }

    Matrix4 Matrix4::zero() {
        Matrix4 matrix(0.0);
        return matrix;
    }

    Matrix4 Matrix4::translation(const Vector &translation) {
        Matrix4 matrix;
        matrix.set(0, 3, translation.x);
        matrix.set(1, 3, translation.y);
        matrix.set(2, 3, translation.z);
        return matrix;
    }

    Matrix4 Matrix4::scale(const Vector &scale) {
        Matrix4 matrix(0.0);
        matrix.set(0, 0, scale.x);
        matrix.set(1, 1, scale.y);
        matrix.set(2, 2, scale.z);
        matrix.set(3, 3, 1.0);
        return matrix;
    }

    Matrix4 Matrix4::shear(double xy, double xz, double yx, double yz,
                           double zx, double zy) {
        Matrix4 matrix;
        matrix.set(0, 1, xy);
        matrix.set(0, 2, xz);
        matrix.set(1, 0, yx);
        matrix.set(1, 2, yz);
        matrix.set(2, 0, zx);
        matrix.set(2, 1, zy);
        return matrix;
    }

    Matrix4 Matrix4::rotationX(double angle) {
        Matrix4 matrix;
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);

        matrix.set(1, 1, cosine);
        matrix.set(1, 2, -sine);
        matrix.set(2, 1, sine);
        matrix.set(2, 2, cosine);
        return matrix;
    }

    Matrix4 Matrix4::rotationY(double angle) {
        Matrix4 matrix;
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);

        matrix.set(0, 0, cosine);
        matrix.set(0, 2, sine);
        matrix.set(2, 0, -sine);
        matrix.set(2, 2, cosine);
        return matrix;
    }

    Matrix4 Matrix4::rotationZ(double angle) {
        Matrix4 matrix;
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);

        matrix.set(0, 0, cosine);
        matrix.set(0, 1, -sine);
        matrix.set(1, 0, sine);
        matrix.set(1, 1, cosine);
        return matrix;
    }

    double Matrix4::at(std::size_t row, std::size_t column) const {
        return _values.at(row).at(column);
    }

    void Matrix4::set(std::size_t row, std::size_t column, double value) {
        _values.at(row).at(column) = value;
    }

    Matrix4 Matrix4::operator*(const Matrix4 &other) const {
        Matrix4 result(0.0);

        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                double value = 0.0;

                for (std::size_t index = 0; index < 4; ++index) {
                    value += _values[row][index] * other._values[index][column];
                }
                result._values[row][column] = value;
            }
        }
        return result;
    }

    Matrix4 &Matrix4::operator*=(const Matrix4 &other) {
        *this = *this * other;
        return *this;
    }

    Vector Matrix4::transformPoint(const Vector &point) const {
        const double x = _values[0][0] * point.x + _values[0][1] * point.y +
                         _values[0][2] * point.z + _values[0][3];
        const double y = _values[1][0] * point.x + _values[1][1] * point.y +
                         _values[1][2] * point.z + _values[1][3];
        const double z = _values[2][0] * point.x + _values[2][1] * point.y +
                         _values[2][2] * point.z + _values[2][3];
        const double w = _values[3][0] * point.x + _values[3][1] * point.y +
                         _values[3][2] * point.z + _values[3][3];

        if (w == 0.0) {
            throw std::runtime_error("Cannot transform point with w equal to 0");
        }
        if (w != 1.0) {
            return Vector(x / w, y / w, z / w);
        }
        return Vector(x, y, z);
    }

    Vector Matrix4::transformVector(const Vector &vector) const {
        return Vector(_values[0][0] * vector.x + _values[0][1] * vector.y +
                          _values[0][2] * vector.z,
                      _values[1][0] * vector.x + _values[1][1] * vector.y +
                          _values[1][2] * vector.z,
                      _values[2][0] * vector.x + _values[2][1] * vector.y +
                          _values[2][2] * vector.z);
    }

    Vector Matrix4::transformNormal(const Vector &normal) const {
        Vector transformed = inverse().transpose().transformVector(normal);
        return transformed.normalized();
    }

    Matrix4 Matrix4::transpose() const {
        Matrix4 result(0.0);

        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                result._values[row][column] = _values[column][row];
            }
        }
        return result;
    }

    Matrix4 Matrix4::inverse() const {
        std::array<std::array<double, 8>, 4> augmented{};

        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                augmented[row][column] = _values[row][column];
            }
            augmented[row][4 + row] = 1.0;
        }

        for (std::size_t column = 0; column < 4; ++column) {
            std::size_t pivotRow = column;
            double pivotValue = std::fabs(augmented[pivotRow][column]);

            for (std::size_t row = column + 1; row < 4; ++row) {
                const double candidate = std::fabs(augmented[row][column]);
                if (candidate > pivotValue) {
                    pivotValue = candidate;
                    pivotRow = row;
                }
            }

            if (pivotValue <= EPSILON) {
                throw std::runtime_error("Matrix is not invertible");
            }
            if (pivotRow != column) {
                std::swap(augmented[pivotRow], augmented[column]);
            }

            const double pivot = augmented[column][column];
            for (double &value : augmented[column]) {
                value /= pivot;
            }

            for (std::size_t row = 0; row < 4; ++row) {
                if (row == column)
                    continue;

                const double factor = augmented[row][column];
                if (factor == 0.0)
                    continue;

                for (std::size_t index = 0; index < 8; ++index) {
                    augmented[row][index] -= factor * augmented[column][index];
                }
            }
        }

        Matrix4 result(0.0);
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                result._values[row][column] = augmented[row][4 + column];
            }
        }
        return result;
    }

    void Matrix4::setIdentity() {
        _values = {};
        for (std::size_t index = 0; index < 4; ++index) {
            _values[index][index] = 1.0;
        }
    }
}  // namespace raytracer::maths