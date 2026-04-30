/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::light {
    DirectionalLight::DirectionalLight(
        const std::map<std::string, std::any> &params)
        : ALight(params) {
        _direction =
            util::Helpers::toVector(params, "direction", "DirectionalLight");

        if (_direction.magnitude() < kMinDistanceEpsilon) {
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

        maths::Ray shadowRay(x + nl * kShadowRayBias, _direction);
        double sT;
        int sId;
        bool occluded = scene.intersect(shadowRay, sT, sId);
        if (!occluded) {
            return _color.toVector() * _intensity * f * cosTheta;
        }
        return maths::Vector(0, 0, 0);
    }
}  // namespace raytracer::object::light