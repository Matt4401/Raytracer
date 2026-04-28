/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Color
*/

#include "math/Color.hpp"

#include <algorithm>
#include <stdexcept>

#include "math/Vector.hpp"

namespace raytracer::maths {

    const Color Color::White{255, 255, 255};
    const Color Color::Black{0, 0, 0};
    const Color Color::Red{255, 0, 0};
    const Color Color::Green{0, 255, 0};
    const Color Color::Blue{0, 0, 255};
    const Color Color::Yellow{255, 255, 0};
    const Color Color::Cyan{0, 255, 255};
    const Color Color::Magenta{255, 0, 255};

    Color::Color(unsigned char color) : r(color), g(color), b(color) {
    }

    Color::Color(unsigned char r, unsigned char g, unsigned char b)
        : r(r), g(g), b(b) {
    }

    Color::Color() : r(0), g(0), b(0) {
    }

    Color Color::operator+(const Color &other) const {
        return Color(std::min(255, this->r + other.r),
                     std::min(255, this->g + other.g),
                     std::min(255, this->b + other.b));
    }

    Color Color::operator*(const Color &other) const {
        return Color((this->r * other.r) / 255, (this->g * other.g) / 255,
                     (this->b * other.b) / 255);
    }

    Color Color::operator*(double scalar) const {
        return Color(std::min(255, static_cast<int>(this->r * scalar)),
                     std::min(255, static_cast<int>(this->g * scalar)),
                     std::min(255, static_cast<int>(this->b * scalar)));
    }

    Color Color::operator/(double scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Color(std::min(255, static_cast<int>(this->r / scalar)),
                     std::min(255, static_cast<int>(this->g / scalar)),
                     std::min(255, static_cast<int>(this->b / scalar)));
    }

    Color &Color::operator+=(const Color &other) {
        this->r = std::min(255, this->r + other.r);
        this->g = std::min(255, this->g + other.g);
        this->b = std::min(255, this->b + other.b);
        return *this;
    }

    Color &Color::operator*=(const Color &other) {
        this->r = (this->r * other.r) / 255;
        this->g = (this->g * other.g) / 255;
        this->b = (this->b * other.b) / 255;
        return *this;
    }

    Color &Color::operator*=(double scalar) {
        this->r = std::min(255, static_cast<int>(this->r * scalar));
        this->g = std::min(255, static_cast<int>(this->g * scalar));
        this->b = std::min(255, static_cast<int>(this->b * scalar));
        return *this;
    }

    Color &Color::operator/=(double scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        this->r = std::min(255, static_cast<int>(this->r / scalar));
        this->g = std::min(255, static_cast<int>(this->g / scalar));
        this->b = std::min(255, static_cast<int>(this->b / scalar));
        return *this;
    }

    Color &Color::operator=(const Color &other) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        return *this;
    }

    bool Color::operator==(const Color &other) const {
        return this->r == other.r && this->g == other.g && this->b == other.b;
    }

    bool Color::operator!=(const Color &other) const {
        return !(*this == other);
    }

    Vector Color::toVector() const {
        return Vector(static_cast<double>(r) / 255.0,
                      static_cast<double>(g) / 255.0,
                      static_cast<double>(b) / 255.0);
    }

}  // namespace raytracer::maths