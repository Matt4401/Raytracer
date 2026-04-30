/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Camera
*/

#include "Camera.hpp"

#include <cmath>
#include <numbers>

#include "exception/PluginException.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::camera {
    Camera::Camera(const std::map<std::string, std::any> &params)
        : AObject(Type::CAMERA),
          _origin(),
          _position(util::ObjectMiddleware::validate<maths::Vector>(
              params, "position", "Camera")),
          _rotation(util::ObjectMiddleware::validate<maths::Vector>(
              params, "rotation", "Camera")),
          _viewport(),
          _fod(0),
          _aspectRatio(1.0) {
        const auto &resolution =
            util::ObjectMiddleware::requireMap(params, "resolution", "camera");
        const double width = util::ObjectMiddleware::validate<double>(
            resolution, "width", "camera");
        const double height = util::ObjectMiddleware::validate<double>(
            resolution, "height", "camera");
        util::ObjectMiddleware::unsignedDouble(width, "width", "camera");
        util::ObjectMiddleware::unsignedDouble(height, "height", "camera");

        const double fieldOfViewDegrees =
            util::ObjectMiddleware::validate<double>(params, "fieldOfView",
                                                     "camera");
        const double fieldOfViewRadians =
            fieldOfViewDegrees * std::numbers::pi / 180.0;

        _origin = _position;
        setViewport(fieldOfViewRadians, width / height);
    }

    Camera::Camera(const maths::Vector &origin, const maths::Vector &position,
                   const maths::Vector &rotation, double fieldOfView,
                   double aspectRatio)
        : AObject(Type::CAMERA),
          _origin(origin),
          _position(position),
          _rotation(rotation),
          _viewport(),
          _fod(0),
          _aspectRatio(1.0) {
        setViewport(fieldOfView, aspectRatio);
    }

    Camera::Camera()
        : AObject(Type::CAMERA),
          _origin(),
          _position(),
          _rotation(),
          _viewport(),
          _fod(0),
          _aspectRatio(1.0) {
    }

    void Camera::setViewport(double fieldOfView, double aspectRatio) {
        _fod = fieldOfView;
        _aspectRatio = aspectRatio;
        const double viewportHeight = 2.0 * std::tan(fieldOfView / 2.0);
        const double viewportWidth = aspectRatio * viewportHeight;
        const maths::Vector horizontal(viewportWidth, 0, 0);
        const maths::Vector vertical(0, viewportHeight, 0);
        const maths::Vector viewportCenter =
            _position + maths::Vector(0, 0, -1);
        _viewport =
            maths::Rectangle(viewportCenter - horizontal / 2 - vertical / 2,
                             horizontal, vertical);
    }

    maths::Ray Camera::ray(double u, double v) const {
        const maths::Vector rayPoint = _viewport.pointAt(u, v) - _position;
        const maths::Vector rayDirection(rayPoint.x, rayPoint.y, rayPoint.z);
        return maths::Ray(_position, rayDirection.normalized());
    }

    void Camera::move(const maths::Vector &direction) {
        _position += direction;
        setViewport(_fod, _aspectRatio);
    }

    void Camera::rotate(const maths::Vector &rotation) {
        _rotation += rotation;
    }

    void Camera::setPosition(const maths::Vector &position) {
        _position = position;
        setViewport(_fod, _aspectRatio);
    }

    void Camera::setRotation(const maths::Vector &rotation) {
        _rotation = rotation;
    }

    maths::Vector Camera::position() const {
        return _position;
    }

    maths::Vector Camera::rotation() const {
        return _rotation;
    }

    maths::Vector Camera::direction() const {
        const double yaw = _rotation.y;
        const double pitch = _rotation.x;
        const double x = std::cos(pitch) * std::sin(yaw);
        const double y = std::sin(pitch);
        const double z = std::cos(pitch) * std::cos(yaw);
        return maths::Vector(x, y, z).normalized();
    }

}  // namespace raytracer::object::camera