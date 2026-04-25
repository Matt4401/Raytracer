/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Vector
*/

#include "../../include/math/Vector.hpp"

namespace raytracer::maths {

    Vector &Vector::operator=(const Vector &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }
    Vector Vector::operator+(const double &scalar) const {
        return Vector(this->x + scalar, this->y + scalar, this->z + scalar);
    }
    Vector Vector::operator-(const double &scalar) const {
        return Vector(this->x - scalar, this->y - scalar, this->z - scalar);
    }
    Vector Vector::operator*(const double &scalar) const {
        return Vector(this->x * scalar, this->y * scalar, this->z * scalar);
    }
    Vector Vector::operator/(const double &scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Vector(this->x / scalar, this->y / scalar, this->z / scalar);
    }
    Vector &Vector::operator+=(const double &scalar) {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;
        return *this;
    }
    Vector &Vector::operator-=(const double &scalar) {
        this->x -= scalar;
        this->y -= scalar;
        this->z -= scalar;
        return *this;
    }
    Vector &Vector::operator*=(const double &scalar) {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }
    Vector &Vector::operator/=(const double &scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        return *this;
    }

    Vector Vector::operator+(const Vector &other) const {
        return Vector(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    Vector Vector::operator-(const Vector &other) const {
        return Vector(this->x - other.x, this->y - other.y, this->z - other.z);
    }
    Vector Vector::operator*(const Vector &other) const {
        return Vector(this->x * other.x, this->y * other.y, this->z * other.z);
    }
    Vector Vector::operator/(const Vector &other) const {
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Vector(this->x / other.x, this->y / other.y, this->z / other.z);
    }
    Vector &Vector::operator+=(const Vector &other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    Vector &Vector::operator-=(const Vector &other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }
    Vector &Vector::operator*=(const Vector &other) {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }
    Vector &Vector::operator/=(const Vector &other) {
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            throw std::runtime_error("Division by zero");
        }
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

    bool Vector::operator==(const Vector &other) const {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }
    bool Vector::operator!=(const Vector &other) const {
        return !(*this == other);
    }

    double Vector::dot(const Vector &other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    Vector Vector::cross(const Vector &other) const {
        return Vector(this->y * other.z - this->z * other.y,
                      this->z * other.x - this->x * other.z,
                      this->x * other.y - this->y * other.x);
    }

    double Vector::magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y +
                         this->z * this->z);
    }

    double Vector::lenght() const {
        return magnitude();
    }

    Vector Vector::normalized() const {
        double mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        return *this / mag;
    }

    void Vector::normalize() {
        double mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        *this /= mag;
    }

    double Vector::distance(const Vector &other) const {
        return (*this - other).magnitude();
    }

    double Vector::angle(const Vector &other) const {
        double dotProduct = dot(other);
        double magProduct = magnitude() * other.magnitude();
        if (magProduct == 0) {
            throw std::runtime_error(
                "Cannot calculate angle with a zero vector");
        }
        double cosTheta = dotProduct / magProduct;
        return std::acos(cosTheta);
    }

    Vector Vector::project(const Vector &other) const {
        double dotProduct = dot(other);
        double magSquared = other.magnitude() * other.magnitude();
        if (magSquared == 0) {
            throw std::runtime_error("Cannot project onto a zero vector");
        }
        return other * (dotProduct / magSquared);
    }

    Vector Vector::translate(const Vector &translation) const {
        return *this + translation;
    }

}  // namespace raytracer::maths
