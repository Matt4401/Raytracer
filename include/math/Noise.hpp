/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Noise
*/

#pragma once

#include <cmath>

namespace raytracer::maths {
    class Noise {
      public:
        Noise(unsigned int seed = 0);
        ~Noise() = default;

        static double fade(double t);
        static double lerp(double a, double b, double t);
        static double grad(int hash, double x, double y, double z);
        static double perlin(double x, double y, double z);
        static std::pair<double, double> worley(double x, double y, double z);
    };
}  // namespace raytracer::maths