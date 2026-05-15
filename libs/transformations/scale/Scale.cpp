/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Scale primitive decorator
*/

#include "Scale.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Scale::Scale(const std::map<std::string, std::any> &params)
        : ATransformDecorator(Type::TRANSFORM) {
        const auto objectPtr =
            util::ObjectMiddleware::validate<std::shared_ptr<object::IObject>>(
                params, "primitive", "scale");
        _wrapped = std::dynamic_pointer_cast<IPrimitive>(objectPtr);

        if (!_wrapped) {
            throw exception::PluginException(
                "Scale requires a primitive object");
        }

        const double x =
            util::ObjectMiddleware::validate<double>(params, "x", "scale");
        const double y =
            util::ObjectMiddleware::validate<double>(params, "y", "scale");
        const double z =
            util::ObjectMiddleware::validate<double>(params, "z", "scale");

        const maths::Vector scaleVec(x, y, z);
        _matrix = maths::Matrix4::scale(scaleVec);
        _inverse = _matrix.inverse();
        _inverseTranspose = _inverse.transpose();
    }
}  // namespace raytracer::object::primitive
