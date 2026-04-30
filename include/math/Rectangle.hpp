/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Rectangle
*/

#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_

#include "Vector.hpp"

namespace raytracer::maths {
    class Rectangle {
      public:
        Rectangle(const Vector &origin, const Vector &bottomSide,
                  const Vector &leftSide);
        Rectangle();
        ~Rectangle() = default;

        /// @brief Calculate the point on the rectangle corresponding to the
        /// given u and v coordinates. The rectangle is defined by its origin
        /// point and two side vectors. The u coordinate corresponds to the
        /// horizontal direction along the bottom side vector, while the v
        /// coordinate corresponds to the vertical direction along the left side
        /// vector. The point is calculated using the formula: point = origin +
        /// u * bottomSide + v * leftSide.
        /// @param u expected to be in the range [0, 1], where 0 corresponds to
        /// the left edge and 1 corresponds to the right edge of the rectangle
        /// @param v expected to be in the range [0, 1], where 0 corresponds to
        /// the bottom edge and 1 corresponds to the top edge of the rectangle
        /// @return the point at the given u and v coordinates on the rectangle
        Vector pointAt(const double u, const double v) const;

      protected:
      private:
        Vector _origin;
        Vector _bottomSide;
        Vector _leftSide;
    };
}  // namespace raytracer::maths

#endif /* !RECTANGLE_HPP_ */
