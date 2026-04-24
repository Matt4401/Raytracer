/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Ray
*/

#ifndef RAY_HPP_
#define RAY_HPP_

#include "Vector.hpp"

namespace raytracer::maths {
    class Ray {
      public:
        Ray() : origin(), direction() {};
        Ray(const Vector &origin, const Vector &direction)
            : origin(origin), direction(direction) {};
        ~Ray() = default;

        Vector origin;
        Vector direction;

      protected:
      private:
    };
}  // namespace raytracer::maths

#endif /* !RAY_HPP_ */
