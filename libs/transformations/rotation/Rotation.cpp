/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Rotation primitive decorator
*/

#include "Rotation.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Rotation::Rotation(const std::map<std::string, std::any> &params)
        : ATransformDecorator(Type::TRANSFORM) {
        const auto objectPtr =
            util::ObjectMiddleware::validate<std::shared_ptr<object::IObject>>(
                params, "primitive", "Rotation");
        _wrapped = std::dynamic_pointer_cast<IPrimitive>(objectPtr);

        if (!_wrapped) {
            throw exception::PluginException(
                "Rotation requires a primitive object");
        }

        const double degX = util::ObjectMiddleware::optional<double>(
            params, "x", 0.0, "Rotation");
        const double degY = util::ObjectMiddleware::optional<double>(
            params, "y", 0.0, "Rotation");
        const double degZ = util::ObjectMiddleware::optional<double>(
            params, "z", 0.0, "Rotation");

        const double radX = degX * M_PI / 180.0;
        const double radY = degY * M_PI / 180.0;
        const double radZ = degZ * M_PI / 180.0;

        const maths::Matrix4 rotX = maths::Matrix4::rotationX(radX);
        const maths::Matrix4 rotY = maths::Matrix4::rotationY(radY);
        const maths::Matrix4 rotZ = maths::Matrix4::rotationZ(radZ);

        _matrix = rotZ * rotY * rotX;
        _inverse = _matrix.inverse();
        _inverseTranspose = _inverse.transpose();
    }
}  // namespace raytracer::object::primitive
