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
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::camera {
    Camera::Camera(const std::map<std::string, std::any> &params)
        : ACamera(maths::Vector(), maths::Vector()) {
        const auto &resolution =
            util::ObjectMiddleware::requireMap(params, "resolution", "Camera");
        const int width = util::ObjectMiddleware::validate<int>(
            resolution, "width", "Camera");
        const int height = util::ObjectMiddleware::validate<int>(
            resolution, "height", "Camera");

        _position = util::Helpers::toVector(params, "position", "Camera");
        _rotation = util::Helpers::toVector(params, "rotation", "Camera");
        util::Helpers::unsignedInt(width, "width", "Camera");
        util::Helpers::unsignedInt(height, "height", "Camera");

        if (width == 0 || height == 0) {
            throw exception::PluginException{
                "Camera resolution width and height must be greater than zero"};
        }

        const double fieldOfViewDegrees =
            util::ObjectMiddleware::validate<double>(params, "fieldOfView",
                                                     "Camera");
        const double fieldOfViewRadians =
            fieldOfViewDegrees * std::numbers::pi / 180.0;

        _imageWidth = width;
        _imageHeight = height;
        setViewport(fieldOfViewRadians, static_cast<double>(width) /
                           static_cast<double>(height));
    }

    Camera::Camera(const maths::Vector &position, const maths::Vector &rotation,
                   const double fieldOfView, const double aspectRatio)
        : ACamera(position, rotation, fieldOfView, aspectRatio) {
    }

    void Camera::setViewport(const double fieldOfView,
                             const double aspectRatio) {
        _fov = fieldOfView;
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

    maths::Ray Camera::ray(const double u, const double v) const {
        const maths::Vector rayPoint = _viewport.pointAt(u, v) - _position;
        const maths::Vector rayDirection(rayPoint.x, rayPoint.y, rayPoint.z);
        return maths::Ray(_position, rayDirection.normalized());
    }

}  // namespace raytracer::object::camera