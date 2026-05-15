/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Shear primitive decorator
*/

#include "Shear.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Shear::Shear(const std::map<std::string, std::any> &params)
        : ATransformDecorator(Type::TRANSFORM) {
        const auto objectPtr =
            util::ObjectMiddleware::validate<std::shared_ptr<object::IObject>>(
                params, "primitive", "shear");
        _wrapped = std::dynamic_pointer_cast<IPrimitive>(objectPtr);

        if (!_wrapped) {
            throw exception::PluginException(
                "Shear requires a primitive object");
        }

        const double xy = util::ObjectMiddleware::optional<double>(
            params, "xy", 0.0, "shear");
        const double xz = util::ObjectMiddleware::optional<double>(
            params, "xz", 0.0, "shear");
        const double yx = util::ObjectMiddleware::optional<double>(
            params, "yx", 0.0, "shear");
        const double yz = util::ObjectMiddleware::optional<double>(
            params, "yz", 0.0, "shear");
        const double zx = util::ObjectMiddleware::optional<double>(
            params, "zx", 0.0, "shear");
        const double zy = util::ObjectMiddleware::optional<double>(
            params, "zy", 0.0, "shear");

        _matrix = maths::Matrix4::shear(xy, xz, yx, yz, zx, zy);
        _inverse = _matrix.inverse();
        _inverseTranspose = _inverse.transpose();
    }
}  // namespace raytracer::object::primitive
