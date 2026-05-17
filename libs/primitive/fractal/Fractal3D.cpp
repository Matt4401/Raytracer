/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Fractal3D
*/

#include "fractal/Fractal3D.hpp"

#include <algorithm>
#include <any>
#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <numbers>
#include <string>

#include "fractal/FractalBuilder.hpp"
#include "object/primitive/APrimitive.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Fractal3D::Fractal3D(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Fractal3D", util::Helpers::toVector(args, "center", "Fractal3D"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Fractal3D")),
          _strategy(fractal::buildMandelbulbStrategy(args, "Fractal3D")),
          _maxSteps(static_cast<int>(util::ObjectMiddleware::validate<double>(
              args, "maxSteps", "Fractal3D"))),
          _epsilon(util::ObjectMiddleware::validate<double>(args, "epsilon",
                                                            "Fractal3D")),
          _maxDist(util::ObjectMiddleware::validate<double>(args, "maxDist",
                                                            "Fractal3D")) {
    }

    Fractal3D::Fractal3D(std::shared_ptr<fractal::IFractalStrategy> strategy,
                         const maths::Vector &center, const int maxSteps,
                         const double epsilon, const double maxDist)
        : Fractal3D(nullptr, std::move(strategy), center, maxSteps, epsilon,
                    maxDist) {
    }

    Fractal3D::Fractal3D(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        std::shared_ptr<fractal::IFractalStrategy> strategy,
        const maths::Vector &center, const int maxSteps, const double epsilon,
        const double maxDist)
        : APrimitive("Fractal3D", center, std::move(material)),
          _strategy(std::move(strategy)),
          _maxSteps(maxSteps),
          _epsilon(epsilon),
          _maxDist(maxDist) {
    }

    bool Fractal3D::hits(const maths::Ray &ray, HitRecord &record) const {
        const double mag = ray.direction.magnitude();
        if (mag < 1e-9)
            return false;
        const maths::Vector dir = ray.direction / mag;
        const maths::Vector oc = ray.origin - _center;
        const double R = _strategy->boundingRadius();
        const double b = oc.dot(dir);
        const double c = oc.dot(oc) - R * R;
        const double disc = b * b - c;

        if (disc < 0.0)
            return false;

        const double sqrtDisc = std::sqrt(disc);
        const double t_enter = -b - sqrtDisc;
        const double t_exit = -b + sqrtDisc;

        if (t_exit < K_RAY_EPSILON)
            return false;

        double t = std::max(t_enter, K_RAY_EPSILON);

        for (int step = 0; step < _maxSteps; ++step) {
            if (t > t_exit || t > _maxDist)
                break;

            const maths::Vector p = oc + dir * t;
            const double de = _strategy->distanceEstimate(p);

            if (de < _epsilon) {
                record.t = t / mag;
                record.hitPoint = ray.origin + ray.direction * record.t;
                record.objectId = id();
                return true;
            }

            t += std::max(de, _epsilon * 0.5);  // avoidinfinite loop
        }

        return false;
    }

    IPrimitive::AABoundingBox Fractal3D::boundingBox() {
        const double r = _strategy->boundingRadius();
        return maths::AABoundingBox{.x = _center.x - r,
                                    .y = _center.y - r,
                                    .z = _center.z - r,
                                    .w = 2.0 * r,
                                    .h = 2.0 * r,
                                    .d = 2.0 * r};
    }

    maths::Vector Fractal3D::computeNormal(
        const maths::Vector &localPoint) const noexcept {
        constexpr double H = NORMAL_H;
        const auto &de = *_strategy;
        const maths::Vector grad(
            de.distanceEstimate(localPoint + maths::Vector(H, 0, 0)) -
                de.distanceEstimate(localPoint + maths::Vector(-H, 0, 0)),

            de.distanceEstimate(localPoint + maths::Vector(0, H, 0)) -
                de.distanceEstimate(localPoint + maths::Vector(0, -H, 0)),

            de.distanceEstimate(localPoint + maths::Vector(0, 0, H)) -
                de.distanceEstimate(localPoint + maths::Vector(0, 0, -H)));

        if (grad.magnitude() < 1e-9)
            return maths::Vector(0.0, 1.0, 0.0);
        return grad.normalized();
    }

    SurfaceData Fractal3D::surfaceData(const HitRecord &record) const {
        const maths::Vector localHit = record.hitPoint - _center;
        const maths::Vector normal = computeNormal(localHit);
        const double len = localHit.magnitude();
        const double u =
            0.5 + std::atan2(localHit.z, localHit.x) / (2.0 * std::numbers::pi);
        const double v =
            (len > 1e-9)
                ? 0.5 - std::asin(std::clamp(localHit.y / len, -1.0, 1.0)) /
                            std::numbers::pi
                : 0.5;
        SurfaceData data{
            .normal = normal, .uv = maths::Vector(u, v, 0.0), .material = {}};

        if (this->_material)
            data.material = this->_material->evaluate(data, record.hitPoint);
        return data;
    }
}  // namespace raytracer::object::primitive
