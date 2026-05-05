/*
** EPITECH PROJECT, 2026
** bsRT
** File description:
** Camera
*/

#pragma once

#include <any>
#include <map>

#include "math/Ray.hpp"
#include "math/Rectangle.hpp"
#include "math/Vector.hpp"
#include "object/ACamera.hpp"

namespace raytracer::object::camera {
    class Camera : public ACamera {
      public:
        explicit Camera(const std::map<std::string, std::any> &params);
        Camera(const maths::Vector &position, const maths::Vector &rotation,
               double fieldOfView, double aspectRatio);
        Camera() = default;
        ~Camera() override = default;

        /// @brief Set the camera's viewport based on the given field of view
        /// and aspect ratio. The viewport is defined as a rectangle in 3D
        /// space, which is calculated based on the camera's position, rotation,
        /// and field of view. The field of view is expected to be in radians
        /// and should be a positive value less than pi. The aspect ratio is
        /// expected to be a positive value representing the ratio of the
        /// viewport's width to its height.
        /// @param fieldOfView the vertical field of view of the camera in
        /// radians
        /// @param aspectRatio the aspect ratio of the viewport, defined as the
        /// ratio of the viewport's width to its height
        void setViewport(double fieldOfView, double aspectRatio) override;
        /// @brief Generate a ray from the camera to the given u and v
        /// coordinates on the image plane. The camera is defined by its
        /// position, rotation, and field of view. The image plane is defined by
        /// a rectangle in 3D space, which is calculated based on the camera's
        /// position, rotation, and field of view.
        /// @param u expected to be in the range [0, 1], where 0 corresponds to
        /// the left edge and 1 corresponds to the right edge of the viewport
        /// @param v expected to be in the range [0, 1], where 0 corresponds to
        /// the bottom edge and 1 corresponds to the top edge of the viewport
        /// @return return a ray, going from the camera to the coordinates and
        /// fov of the viewport.
        maths::Ray ray(const double u, const double v) const override;

      protected:
      private:
        maths::Rectangle _viewport;
    };
}  // namespace raytracer::object::camera
