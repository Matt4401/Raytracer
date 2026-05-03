/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ACamera
*/

#pragma once

#include <numbers>
#include <string>

#include "AObject.hpp"
#include "ICamera.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::camera {
    class ACamera : public ICamera, public AObject {
      public:
        ACamera(maths::Vector origin, maths::Vector rotation,
                double fov = std::numbers::pi / 2,
                double aspectRatio = 16.0 / 9.0)
            : AObject(object::AObject::Type::CAMERA),
              _fov(fov),
              _aspectRatio(aspectRatio),
              _position(origin),
              _rotation(rotation),
              _imageWidth(800),
              _imageHeight(600) {};
        ACamera()
            : AObject(object::AObject::Type::CAMERA),
              _fov(std::numbers::pi / 2),
              _aspectRatio(16.0 / 9.0),
              _position(),
              _rotation(),
              _imageWidth(800),
              _imageHeight(600) {};
        ~ACamera() = default;
        virtual maths::Ray ray(double u, double v) const override = 0;
        virtual void setViewport(double fieldOfView,
                                 double aspectRatio) override = 0;

        void setImageWidth(int width) override {
            _imageWidth = width;
        }
        void setImageHeight(int height) override {
            _imageHeight = height;
        }
        void setPosition(const maths::Vector &position) override {
            _position = position;
        }
        void setRotation(const maths::Vector &rotation) override {
            _rotation = rotation;
        }
        void setFOV(double fov) override {
            _fov = fov;
        }
        void setAspectRatio(double aspectRatio) override {
            _aspectRatio = aspectRatio;
        }

        int imageWidth() const override {
            return _imageWidth;
        }
        int imageHeight() const override {
            return _imageHeight;
        }
        double fov() const override {
            return _fov;
        }
        double aspectRatio() const override {
            return _aspectRatio;
        }
        maths::Vector position() const override {
            return _position;
        }
        maths::Vector rotation() const override {
            return _rotation;
        }

      protected:
        double _fov = std::numbers::pi / 2;
        double _aspectRatio = 16.0 / 9.0;
        maths::Vector _position;
        maths::Vector _rotation;
        int _imageWidth = 800;
        int _imageHeight = 600;

      private:
    };
}  // namespace raytracer::object::camera
