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
        ACamera(const maths::Vector origin, const maths::Vector rotation,
                const double fov = std::numbers::pi / 2,
                const double aspectRatio = 16.0 / 9.0)
            : AObject(object::AObject::Type::CAMERA),
              _fov(fov),
              _aspectRatio(aspectRatio),
              _position(origin),
              _rotation(rotation),
              _imageWidth(kImageWidth),
              _imageHeight(kImageHeight) {};
        ACamera()
            : AObject(object::AObject::Type::CAMERA),
              _fov(std::numbers::pi / 2),
              _aspectRatio(16.0 / 9.0),
              _position(),
              _rotation(),
              _imageWidth(kImageWidth),
              _imageHeight(kImageHeight) {};
        ~ACamera() override = default;
        virtual maths::Ray ray(const double u,
                               const double v) const override = 0;
        virtual void setViewport(const double fieldOfView,
                                 const double aspectRatio) override = 0;

        void setImageWidth(const int width) override {
            _imageWidth = width;
        }
        void setImageHeight(const int height) override {
            _imageHeight = height;
        }
        void setPosition(const maths::Vector &position) override {
            _position = position;
        }
        void setRotation(const maths::Vector &rotation) override {
            _rotation = rotation;
        }
        void setFOV(const double fov) override {
            _fov = fov;
        }
        void setAspectRatio(const double aspectRatio) override {
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
        static constexpr std::size_t kImageWidth = 800;
        static constexpr std::size_t kImageHeight = 600;
        double _fov = std::numbers::pi / 2;
        double _aspectRatio = 16.0 / 9.0;
        maths::Vector _position;
        maths::Vector _rotation;
        int _imageWidth = kImageWidth;
        int _imageHeight = kImageHeight;

      private:
    };
}  // namespace raytracer::object::camera
