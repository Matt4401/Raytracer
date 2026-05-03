/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ICamera
*/

#pragma once

#include "math/Ray.hpp"

namespace raytracer::object::camera {
    class ICamera {
      public:
        ICamera() = default;
        virtual ~ICamera() = default;

        virtual void setViewport(double fieldOfView, double aspectRatio) = 0;
        virtual maths::Ray ray(double u, double v) const = 0;

        virtual void setImageWidth(int width) = 0;
        virtual void setImageHeight(int height) = 0;

        virtual void setPosition(const maths::Vector &position) = 0;
        virtual void setRotation(const maths::Vector &rotation) = 0;
        virtual void setFOV(double fov) = 0;
        virtual void setAspectRatio(double aspectRatio) = 0;

        virtual int imageWidth() const = 0;
        virtual int imageHeight() const = 0;
        virtual double fov() const = 0;
        virtual double aspectRatio() const = 0;
        virtual maths::Vector position() const = 0;
        virtual maths::Vector rotation() const = 0;

      protected:
      private:
    };
}  // namespace raytracer::object::camera
