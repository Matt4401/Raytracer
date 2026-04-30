/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpotLight
*/

#include "SpotLight.hpp"

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::light {
    SpotLight::SpotLight(const std::map<std::string, std::any> &params)
        : ALight(params) {
        const auto &direction = util::ObjectMiddleware::requireMap(
            params, "direction", "SpotLight");
        _direction =
            util::Helpers::toVector(direction, "direction", "SpotLight");
        const double exponent = util::ObjectMiddleware::validate<double>(
            params, "exponent", "SpotLight");
        util::Helpers::unsignedDouble(exponent, "exponent", "SpotLight");
        _exponent = exponent;
    }

    maths::Vector SpotLight::computeNEE(const scene::IScene &scene,
                                        const maths::Vector &x,
                                        const maths::Vector &nl,
                                        const maths::Vector &f) const {
        maths::Vector toL(_position.x - x.x, _position.y - x.y,
                          _position.z - x.z);
        double dist = toL.magnitude();
        if (dist < 1e-6)
            return maths::Vector(0, 0, 0);
        maths::Vector ldir = toL / dist;
        double cosTheta = nl.dot(ldir.normalized());
        if (cosTheta <= 0)
            return maths::Vector(0, 0, 0);

        maths::Ray shadowRay(x + nl * 1e-4, ldir);
        double sT;
        int sId;
        bool hit = scene.intersect(shadowRay, sT, sId);

        if (!hit || sT > dist - 1e-3) {
            double spotEffect =
                std::pow(std::max(0.0, ldir.dot(_direction * -1.0)), _exponent);
            double att = spotEffect / (dist * dist);
            return _color.toVector() * _intensity * f * cosTheta * att;
        }
        return maths::Vector(0, 0, 0);
    }

}  // namespace raytracer::object::light