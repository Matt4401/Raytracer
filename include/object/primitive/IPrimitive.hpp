/*
** EPITECH PROJECT, 2026
** IPrimitive
** File description:
** IPrimitive header
*/

#pragma once

#include <string>
#include <unordered_map>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::primitive {
    struct SurfaceData {
        maths::Vector normal;
        maths::Vector uv;
        maths::Color color;
        maths::Vector emission;
        RefltT reflType;
        std::unordered_map<std::string, double> extraParams;
    };

    class IPrimitive {
      public:
        struct BoundingBox {
            double x;
            double y;
            double z;
            double w;
            double h;
            double d;
        };

        virtual ~IPrimitive() = default;
        /**
         *
         * @param ray the ray to test for intersection with the primitive. The
         * ray is expected to be a valid Ray object with defined origin and
         * direction components. The origin represents the starting point of the
         * ray in 3D space, while the direction represents the vector along
         * which the ray is cast. The hits function will calculate if and where
         * the ray intersects with the primitive, returning a double value that
         * represents the distance from the ray's origin to the point of
         * intersection.
         * @return the distance from the ray's origin to the point of
         * intersection with the primitive, or a specific value if no
         * intersection occurs
         */
        virtual double hits(const maths::Ray &ray) = 0;

        /**
         * @brief Get surface data at hit point (normal, uv, etc.) and evaluates
         * underlying materials for color, emission, etc.
         */
        virtual SurfaceData surfaceData(
            const maths::Vector &hitPoint) const = 0;

        /**
         *
         * @return a BoundingBox struct that defines the axis-aligned bounding
         * box of the primitive. The bounding box is defined by its position (x,
         * y, z) and its dimensions (w, h, d). The position represents the
         * coordinates of the minimum corner of the bounding box in 3D space,
         * while the dimensions represent the width (w), height (h), and depth
         * (d) of the bounding box along the x, y, and z axes, respectively. The
         * bounding box is used for efficient spatial partitioning and collision
         * detection in ray tracing algorithms, allowing for faster intersection
         * tests by quickly determining if a ray intersects with the bounding
         * box before performing more complex intersection calculations with the
         * primitive itself.
         */
        virtual BoundingBox boundingBox() = 0;

        virtual const std::string &name() const noexcept = 0;
        virtual maths::Vector center() const noexcept = 0;
    };

}  // namespace raytracer::object::primitive
