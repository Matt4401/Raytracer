/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ICamera
*/

#ifndef ICAMERA_HPP_
#define ICAMERA_HPP_

#include "Ray.hpp"

namespace raytracer::object::camera {
    class ICamera {
      public:
        ICamera() = default;
        ~ICamera() = default;
        virtual maths::Ray ray(double u, double v) const = 0;
        virtual void setPosition(const maths::Vector &position) = 0;
        virtual void setRotation(const maths::Vector &rotation) = 0;
        virtual void setFOV(double fov) = 0;
        virtual void setAspectRatio(double aspectRatio) = 0;

        virtual double fov() const = 0;
        virtual double aspectRatio() const = 0;
        virtual maths::Vector position() const = 0;
        virtual maths::Vector rotation() const = 0;

      protected:
      private:
    };
}  // namespace raytracer::object::camera

#endif /* !ICAMERA_HPP_ */
