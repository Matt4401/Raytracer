/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TangleCube
*/

#include "TangleCube.hpp"

#include <algorithm>
#include <any>
#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <numbers>
#include <string>

#include "math/PolynomialSolver.hpp"
#include "object/primitive/APrimitive.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {

    TangleCube::TangleCube(const std::map<std::string, std::any> &args)
        : APrimitive(
              "TangleCube",
              util::Helpers::toVector(args, "center", "TangleCube"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "TangleCube")),
          _constant(util::ObjectMiddleware::validate<double>(args, "constant",
                                                             "TangleCube")) {
    }

    TangleCube::TangleCube(const maths::Vector &center, const double constant)
        : TangleCube(nullptr, center, constant) {
    }

    TangleCube::TangleCube(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        const maths::Vector &center, const double constant)
        : APrimitive("TangleCube", center, std::move(material)),
          _constant(constant) {
    }

    const double &TangleCube::constant() const noexcept {
        return _constant;
    }

    bool TangleCube::hits(const maths::Ray &ray, HitRecord &record) const {
        const double mag = ray.direction.magnitude();
        if (mag < 1e-9)
            return false;
        const maths::Vector d = ray.direction / mag;
        const maths::Vector o = ray.origin - _center;
        const double ox2 = o.x * o.x, oy2 = o.y * o.y, oz2 = o.z * o.z;
        const double dx2 = d.x * d.x, dy2 = d.y * d.y, dz2 = d.z * d.z;

        const double ox3 = ox2 * o.x, oy3 = oy2 * o.y, oz3 = oz2 * o.z;
        const double dx3 = dx2 * d.x, dy3 = dy2 * d.y, dz3 = dz2 * d.z;
        const double ox4 = ox2 * ox2, oy4 = oy2 * oy2, oz4 = oz2 * oz2;
        const double dx4 = dx2 * dx2, dy4 = dy2 * dy2, dz4 = dz2 * dz2;
        const double a4 = dx4 + dy4 + dz4;
        const double a3 = 4.0 * (o.x * dx3 + o.y * dy3 + o.z * dz3);
        const double a2 =
            6.0 * (ox2 * dx2 + oy2 * dy2 + oz2 * dz2) - 5.0 * (dx2 + dy2 + dz2);
        const double a1 = 4.0 * (ox3 * d.x + oy3 * d.y + oz3 * d.z) -
                          10.0 * (o.x * d.x + o.y * d.y + o.z * d.z);
        const double a0 = ox4 + oy4 + oz4 - 5.0 * (ox2 + oy2 + oz2) + _constant;
        std::array<double, 4> roots{};
        const int count =
            maths::PolynomialSolver::solveQuartic(a4, a3, a2, a1, a0, roots);

        if (count <= 0)
            return false;
        auto evalP = [&](const double t) noexcept -> double {
            return ((((a4 * t) + a3) * t + a2) * t + a1) * t + a0;
        };
        auto evalDP = [&](const double t) noexcept -> double {
            return (((4.0 * a4 * t) + 3.0 * a3) * t + 2.0 * a2) * t + a1;
        };

        double tMin = std::numeric_limits<double>::infinity();

        for (int i = 0; i < count; ++i) {
            double ti = roots[i];
            if (!std::isfinite(ti))
                continue;
            for (int iter = 0; iter < 5; ++iter) {  // lissage newton
                const double f = evalP(ti);
                const double df = evalDP(ti);
                if (std::fabs(df) < 1e-12)
                    break;
                ti -= f / df;
            }
            if (ti > 0.001 && ti < tMin) {
                tMin = ti;
            }
        }
        if (tMin == std::numeric_limits<double>::infinity())
            return false;
        record.t = tMin / mag;
        record.hitPoint = ray.origin + ray.direction * record.t;
        record.objectId = id();
        return true;
    }

    IPrimitive::AABoundingBox TangleCube::boundingBox() {
        const double disc = 75.0 - 4.0 * _constant;
        double extent;
        if (disc > 0.0) {
            const double x2max = (5.0 + std::sqrt(disc)) / 2.0;
            extent = (x2max > 0.0) ? std::sqrt(x2max) * 1.05 : 2.5;
        } else {
            extent = 2.5;
        }

        return maths::AABoundingBox{.x = _center.x - extent,
                                    .y = _center.y - extent,
                                    .z = _center.z - extent,
                                    .w = 2.0 * extent,
                                    .h = 2.0 * extent,
                                    .d = 2.0 * extent};
    }

    maths::Vector TangleCube::computeNormal(const maths::Vector &localPoint) {
        const maths::Vector grad(
            4.0 * localPoint.x * localPoint.x * localPoint.x -
                10.0 * localPoint.x,
            4.0 * localPoint.y * localPoint.y * localPoint.y -
                10.0 * localPoint.y,
            4.0 * localPoint.z * localPoint.z * localPoint.z -
                10.0 * localPoint.z);

        const double mag = grad.magnitude();
        if (mag < 1e-9)
            return maths::Vector(0.0, 1.0, 0.0);
        return grad / mag;
    }

    SurfaceData TangleCube::surfaceData(const HitRecord &record) const {
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