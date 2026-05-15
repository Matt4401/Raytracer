/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Translation
*/

#include "Translation.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Translation::Translation(const std::map<std::string, std::any> &params)
        : ATransformDecorator(Type::TRANSFORM) {
        const auto objectPtr =
            util::ObjectMiddleware::validate<std::shared_ptr<object::IObject>>(
                params, "primitive", "translation");
        _wrapped = std::dynamic_pointer_cast<IPrimitive>(objectPtr);

        if (!_wrapped) {
            throw exception::PluginException(
                "Translation requires a primitive object");
        }

        const double x = util::ObjectMiddleware::validate<double>(
            params, "x", "translation");
        const double y = util::ObjectMiddleware::validate<double>(
            params, "y", "translation");
        const double z = util::ObjectMiddleware::validate<double>(
            params, "z", "translation");

        const maths::Vector translationVec(x, y, z);
        _matrix = maths::Matrix4::translation(translationVec);
        _inverse = _matrix.inverse();
        _inverseTranspose = _inverse.transpose();
    }
}  // namespace raytracer::object::primitive
