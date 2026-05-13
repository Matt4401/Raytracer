/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BoundingBox
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::maths {
    class AABoundingBox {
      public:
        double x = std::numeric_limits<double>::infinity();
        double y = std::numeric_limits<double>::infinity();
        double z = std::numeric_limits<double>::infinity();
        double w = std::numeric_limits<double>::infinity();
        double h = std::numeric_limits<double>::infinity();
        double d = std::numeric_limits<double>::infinity();

        /// @brief Extend the current bounding box to include another bounding
        /// box. This function modifies the current bounding box so that it
        /// encompasses the volume defined by both the current bounding box and
        /// the other bounding box.
        /// @param other The bounding box to extend the current bounding box to
        /// include. This should be a valid BoundingBox object with defined
        /// position and dimensions.
        /// @return void
        void extend(const AABoundingBox &other);

        /// @brief Extend the current bounding box to include a point.
        /// @param point The position to include in the current bounding box.
        void extend(const Vector &point);

        /// @brief Expand the box uniformly in all directions.
        /// @param amount The padding size to add on each side of the box.
        void pad(double amount);

        /// @brief Calculate the surface area of the bounding box. The surface
        /// area is calculated using the formula: surface area = 2 * (w * h + w
        /// * d + h * d), where w, h, and d are the width, height, and depth of
        /// the bounding box, respectively.
        /// @return the surface area of the bounding box, or 0 if any dimension
        /// is negative
        double surfaceData() const;

        /// @brief Check if a ray intersects with the bounding box. This
        /// function uses the "slab method" to determine if the ray intersects
        /// with the bounding box.
        /// @param ray The ray to test for intersection with the bounding box.
        /// This should be a valid Ray object with defined origin and direction
        /// components.
        /// @return the entry distance of the ray in the box, or -1 if there
        /// is no intersection.
        double intersects(const Ray &ray) const;
    };

}  // namespace raytracer::maths
