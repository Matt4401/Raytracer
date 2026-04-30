/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** PointLight
*/

#include "PointLight.hpp"

#include <algorithm>
#include <cmath>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::light {
    PointLight::PointLight(const std::map<std::string, std::any> &params)
        : ALight(params) {
        const double radius = util::ObjectMiddleware::validate<double>(
            params, "radius", "PointLight");
        util::Helpers::unsignedDouble(radius, "radius", "PointLight");
        _radius = radius;
    }

    maths::Vector PointLight::computeNEE(const scene::IScene &scene,
                                         const maths::Vector &x,
                                         const maths::Vector &nl,
                                         const maths::Vector &f) const {
        maths::Vector toL(position().x - x.x, position().y - x.y,
                          position().z - x.z);
        double dist = toL.magnitude();
        if (dist < kMinDistanceEpsilon)
            return maths::Vector(0, 0, 0);
        maths::Vector ldir = toL / dist;
        double cosTheta = nl.dot(ldir);
        if (cosTheta <= 0)
            return maths::Vector(0, 0, 0);

        maths::Ray shadowRay(x + nl * kShadowRayBias, ldir);
        double sT;
        int sId;
        bool hit = scene.intersect(shadowRay, sT, sId);

        if (_radius > 0.0) {
            if (!hit || sT > dist - _radius - kOcclusionEpsilon) {
                double cosAlpha = std::sqrt(
                    std::max(0.0, 1.0 - (_radius / dist) * (_radius / dist)));
                double omega = 2 * M_PI * (1.0 - cosAlpha);
                return color().toVector() * intensity() * f * cosTheta *
                       (omega / (2.0 * M_PI));
            }
        } else {
            if (!hit || sT > dist - kOcclusionEpsilon) {
                double att = 1.0 / (dist * dist);
                return color().toVector() * intensity() * f * cosTheta * att;
            }
        }
        return maths::Vector(0, 0, 0);
    }
}  // namespace raytracer::object::light