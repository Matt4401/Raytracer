/*
** EPITECH PROJECT, 2026
** bsRT
** File description:
** Camera
*/

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <any>
#include <map>

#include "math/Ray.hpp"
#include "math/Rectangle.hpp"
#include "math/Vector.hpp"
#include "object/AObject.hpp"

namespace raytracer::object::camera {
    class Camera : public raytracer::object::AObject {
      public:
        explicit Camera(const std::map<std::string, std::any> &params);
        Camera(const maths::Vector &origin, const maths::Vector &position,
               const maths::Vector &rotation, double fieldOfView,
               double aspectRatio);
        Camera();
        ~Camera() = default;

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
        void setViewport(double fieldOfView, double aspectRatio);
        /// @brief Generate a ray from the camera to the given u and v
        /// coordinates on the image plane. The camera is defined by its
        /// position, rotation, and field of view. The image plane is defined by
        /// a rectangle in 3D space, which is calculated based on the camera's
        /// position, rotation, and field of view.
        /// @param u expected to be in the range [0, 1], where 0 corresponds to
        /// the left edge and 1 corresponds to the right edge of the viewport
        /// @param v expected to be in the range [0, 1], where 0 corresponds to
        /// the bottom edge and 1 corresponds to the top edge of the viewport
        /// @return return a ray, going from the camera to the coordinates uand
        /// vof the viewport.
        maths::Ray ray(double u, double v) const;

        const std::map<std::string, std::any> &requireMap(
            const std::map<std::string, std::any> &params,
            const std::string_view key, const std::string_view className);

        void move(const maths::Vector &direction);
        void rotate(const maths::Vector &rotation);
        void setPosition(const maths::Vector &position);
        void setRotation(const maths::Vector &rotation);
        maths::Vector position() const;
        maths::Vector rotation() const;
        maths::Vector direction() const;

      protected:
      private:
        maths::Vector _origin;
        maths::Vector _position;
        maths::Vector _rotation;
        maths::Rectangle _viewport;
        double _fod;
        double _aspectRatio;
    };
}  // namespace raytracer::object::camera
#endif /* !CAMERA_HPP_ */
