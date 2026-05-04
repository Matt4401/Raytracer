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
        _direction = util::Helpers::toVector(params, "direction", "SpotLight");
        const double exponent = util::ObjectMiddleware::validate<double>(
            params, "exponent", "SpotLight");
        util::Helpers::unsignedDouble(exponent, "exponent", "SpotLight");
        _exponent = exponent;
    }

    maths::Vector SpotLight::computeNEE(
        const scene::IScene &scene, const maths::Vector &x,
        const maths::Vector &nl,
        const primitive::MaterialProperties &material) const {
        maths::Vector toL(_position.x - x.x, _position.y - x.y,
                          _position.z - x.z);
        double dist = toL.magnitude();
        if (dist < K_MIN_DISTANCE_EPSILON)
            return maths::Vector(0, 0, 0);
        maths::Vector ldir = toL / dist;
        double cosTheta = nl.dot(ldir.normalized());
        if (cosTheta <= 0)
            return maths::Vector(0, 0, 0);

        maths::Ray shadowRay(x + nl * K_SHADOW_RAY_BIAS, ldir);
        double sT;
        int sId;
        bool hit = scene.intersect(shadowRay, sT, sId);

        if (!hit || sT > dist - K_OCCLUSION_EPSILON) {
            double spotEffect =
                std::pow(std::max(0.0, ldir.dot(_direction * -1.0)), _exponent);
            double att = spotEffect / (dist * dist);
            return _color.toVector() * _intensity *
                   materialDiffuseResponse(material) * cosTheta * att;
        }
        return maths::Vector(0, 0, 0);
    }

}  // namespace raytracer::object::light