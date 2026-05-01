/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ACamera
*/

#pragma once

#include <numbers>

#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::camera {
    class ACamera {
      public:
        ACamera(maths::Vector origin, maths::Vector rotation,
                double fov = std::numbers::pi / 2,
                double aspectRatio = 16.0 / 9.0)
            : _fov(fov),
              _aspectRatio(aspectRatio),
              _position(origin),
              _rotation(rotation) {};
        ~ACamera() = default;
        virtual maths::Ray ray(double u, double v) const = 0;
        void setPosition(const maths::Vector &position) {
            _position = position;
        }
        virtual void setRotation(const maths::Vector &rotation) {
            _rotation = rotation;
        }
        virtual void setFOV(double fov) {
            _fov = fov;
        }
        virtual void setAspectRatio(double aspectRatio) {
            _aspectRatio = aspectRatio;
        }

        virtual double fov() const {
            return _fov;
        }
        virtual double aspectRatio() const {
            return _aspectRatio;
        }
        virtual maths::Vector position() const {
            return _position;
        }
        virtual maths::Vector rotation() const {
            return _rotation;
        }

      protected:
        double _fov = std::numbers::pi / 2;
        double _aspectRatio = 16.0 / 9.0;
        maths::Vector _position;
        maths::Vector _rotation;

      private:
    };
}  // namespace raytracer::object::camera
