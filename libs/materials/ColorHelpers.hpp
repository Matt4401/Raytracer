/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ColorHelpers
*/

#pragma once

#include <algorithm>
#include <cmath>

#include "math/Color.hpp"

namespace raytracer::materials::helpers {
    /**
     * Interpolates between two colors based on a blend factor
     * @param color1 The first color (blend factor = 0)
     * @param color2 The second color (blend factor = 1)
     * @param factor The blend factor [0.0, 1.0]
     * @return Interpolated color
     */
    inline maths::Color interpolateColors(const maths::Color& color1,
                                          const maths::Color& color2,
                                          double factor) {
        return maths::Color(
            static_cast<unsigned char>(
                std::clamp(static_cast<int>(std::round(
                               color1.r * (1.0 - factor) + color2.r * factor)),
                           0, 255)),
            static_cast<unsigned char>(
                std::clamp(static_cast<int>(std::round(
                               color1.g * (1.0 - factor) + color2.g * factor)),
                           0, 255)),
            static_cast<unsigned char>(
                std::clamp(
                    static_cast<int>(std::round(
                        color1.b * (1.0 - factor) + color2.b * factor)),
                    0, 255)));
    }
}  // namespace raytracer::materials::helpers
