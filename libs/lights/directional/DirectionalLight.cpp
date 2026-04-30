/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"

#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::light {
    DirectionalLight::DirectionalLight(
        const std::map<std::string, std::any> &params)
        : ALight(params) {
        const auto &direction = util::ObjectMiddleware::requireMap(
            params, "direction", "DirectionalLight");
        _direction = maths::Vector(util::ObjectMiddleware::validate<double>(
                                       direction, "x", "DirectionalLight"),
                                   util::ObjectMiddleware::validate<double>(
                                       direction, "y", "DirectionalLight"),
                                   util::ObjectMiddleware::validate<double>(
                                       direction, "z", "DirectionalLight"));
        _direction.normalize();
    }

    maths::Vector DirectionalLight::computeNEE(const scene::IScene &scene,
                                               const maths::Vector &x,
                                               const maths::Vector &nl,
                                               const maths::Vector &f) const {
        double cosTheta = nl.dot(_direction);
        if (cosTheta <= 0)
            return maths::Vector(0, 0, 0);

        maths::Ray shadowRay(x + nl * 1e-4, _direction);
        double sT;
        int sId;
        bool occluded = scene.intersect(shadowRay, sT, sId);
        if (!occluded) {
            return _color.toVector() * _intensity * f * cosTheta;
        }
        return maths::Vector(0, 0, 0);
    }
}  // namespace raytracer::object::light