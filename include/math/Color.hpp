/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Color
*/

#pragma once

#include "Vector.hpp"

namespace raytracer::maths {
    class Color {
      public:
        explicit Color(unsigned char color);
        Color(unsigned char r, unsigned char g, unsigned char b);
        Color();
        ~Color() = default;

        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;

        Color operator+(const Color &other) const;
        Color operator*(const Color &other) const;
        Color operator*(double scalar) const;
        Color operator/(double scalar) const;
        Color &operator+=(const Color &other);
        Color &operator*=(const Color &other);
        Color &operator*=(double scalar);
        Color &operator/=(double scalar);
        Color &operator=(const Color &other);

        bool operator==(const Color &other) const;
        bool operator!=(const Color &other) const;

        Vector toVector() const;

        unsigned char r;
        unsigned char g;
        unsigned char b;

      protected:
      private:
    };
}  // namespace raytracer::maths
