/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"

namespace raytracer::object::light {
    DirectionalLight::DirectionalLight(
        const std::map<std::string, std::any> &params)
        : ALight(params) {
        _direction =
            util::Helpers::toVector(params, "direction", "DirectionalLight");

        if (_direction.magnitude() < K_MIN_DISTANCE_EPSILON) {
            throw exception::PluginException{
                "DirectionalLight direction vector cannot be zero"};
        }
        _direction.normalize();
    }

    maths::Vector DirectionalLight::computeNEE(const scene::IScene &scene,
                                               const maths::Vector &x,
                                               const maths::Vector &nl,
                                               const maths::Vector &f) const {
        double cosTheta = nl.dot(_direction);
        if (cosTheta <= 0)
            return maths::Vector(0, 0, 0);

        maths::Ray shadowRay(x + nl * K_SHADOW_RAY_BIAS, _direction);
        primitive::HitRecord shadowRecord;
        bool occluded = scene.intersect(shadowRay, shadowRecord);
        if (!occluded) {
            return _color.toVector() * _intensity * f * cosTheta;
        }
        return maths::Vector(0, 0, 0);
    }
}  // namespace raytracer::object::light