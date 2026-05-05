/*
** EPITECH PROJECT, 2026
** Vector
** File description:
** Vector header
*/

#pragma once

#include <cmath>

namespace raytracer::maths {
    class Vector {
      public:
        Vector() : x(0), y(0), z(0) {};
        explicit Vector(double x, double y, double z) : x(x), y(y), z(z) {};
        ~Vector() = default;

        Vector &operator=(const Vector &other);

        Vector operator+(const double &scalar) const;
        Vector operator-(const double &scalar) const;
        Vector operator*(const double &scalar) const;
        Vector operator/(const double &scalar) const;
        Vector &operator+=(const double &scalar);
        Vector &operator-=(const double &scalar);
        Vector &operator*=(const double &scalar);
        Vector &operator/=(const double &scalar);

        Vector operator+(const Vector &other) const;
        Vector operator-(const Vector &other) const;
        Vector operator*(const Vector &other) const;
        Vector operator/(const Vector &other) const;
        Vector &operator+=(const Vector &other);
        Vector &operator-=(const Vector &other);
        Vector &operator*=(const Vector &other);
        Vector &operator/=(const Vector &other);

        bool operator==(const Vector &other) const;
        bool operator!=(const Vector &other) const;

        double dot(const Vector &other) const;

        Vector cross(const Vector &other) const;
        /// @brief Calculate the magnitude (length) of the vector. The magnitude
        /// is calculated using the formula: magnitude = sqrt(x^2 + y^2 + z^2),
        /// where x, y, and z are the components of the vector. The magnitude
        /// represents the distance from the origin to the point defined by the
        /// vector in 3D space.
        /// @return the magnitude of the vector
        double magnitude() const;
        double lenght() const;

        /// @brief Calculate the normalized version of the vector. The
        /// normalized vector is a unit vector that has the same
        /// @return the normalized version of the vector direction as the
        /// original vector but with a magnitude of 1. The normalization is done
        /// by dividing each component of the vector by its magnitude. If the
        /// vector has a magnitude of zero, an exception is thrown to prevent
        /// division by zero.
        Vector normalized() const;
        /// @brief Normalize the vector in place. This function modifies the
        /// original vector to have a magnitude of 1 while maintaining its
        /// direction. The normalization is done by dividing each component of
        /// the vector by its magnitude. If the vector has a magnitude of zero,
        /// an exception is thrown to prevent division by zero.
        void normalize();
        /// @brief Calculate the distance between this vector and another
        /// vector. The distance is calculated as the magnitude of the
        /// difference between the two vectors, which represents the
        /// straight-line distance between the points defined by the vectors in
        /// 3D space. The formula used is: distance = magnitude(this - other),
        /// where "this" is the current vector and "other" is the vector to
        /// which we are calculating the distance.
        /// @param other the vector to which we want to calculate the distance
        /// from the current vector. The "other" vector is expected to be a
        /// valid Vector object with defined x, y, and z components.
        /// @return the distance between the two vectors
        double distance(const Vector &other) const;

        /// @brief Calculate the angle between this vector and another vector.
        /// The angle is calculated using the formula: angle = acos((this .
        /// other) / (magnitude(this) * magnitude(other))), where "this" is the
        /// current vector and "other" is the vector to which we are calculating
        /// the angle. If either vector has a magnitude of zero, an exception is
        /// thrown to prevent division by zero. The angle is returned in radians
        /// and will be in the range [0, pi]. An angle of 0 indicates that the
        /// vectors are pointing in the same direction, while an angle of pi
        /// indicates that the vectors are pointing in opposite directions. An
        /// angle of pi/2 indicates that the vectors are perpendicular to each
        /// other.
        /// @param other the vector to which we want to calculate the angle from
        /// the current vector. The "other" vector is expected to be a valid
        /// Vector object with defined x, y, and z components.
        /// @return the angle between the two vectors in radians
        double angle(const Vector &other) const;

        /// @brief Calculate the reflection of this vector off a surface with
        /// the given normal.
        /// @param normal the normal vector of the surface. The "normal" vector
        /// is expected to be a valid Vector object with defined x, y, and z
        /// components. The normal vector should be normalized (have a magnitude
        /// of 1) for accurate results.
        /// @return the reflected vector
        Vector reflect(const Vector &normal) const {
            return *this - normal * (2 * dot(normal));
        }
        /// @brief Calculate the projection of this vector onto another vector.
        /// The projection is calculated using the formula: projection = (this .
        /// other / other . other) * other, where "this" is the current vector
        /// and "other" is the vector onto which we are projecting.
        /// @param other the vector onto which we want to project the current
        /// vector. The "other" vector is expected to be a valid Vector object
        /// with defined x, y, and z components.
        /// @return the projection of the current vector onto the other vector
        Vector project(const Vector &other) const;

        /// @brief move the vector in the 3D space in the direction (and
        /// distance) specified by the parameter
        /// @param translation the vector specifying the direction and distance
        /// to move. The translation vector is added to the original vector,
        /// resulting in a new vector that represents the translated position in
        /// 3D space.
        /// @return a new vector that is the result of translating the original
        /// vector by the given translation vector
        Vector translate(const Vector &translation) const;

        double x = 0;
        double y = 0;
        double z = 0;
    };

}  // namespace raytracer::maths
