/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Torus
*/

#include "Torus.hpp"

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
    Torus::Torus(const std::map<std::string, std::any> &args)
        : APrimitive(
              "Torus", util::Helpers::toVector(args, "center", "Torus"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  args, "material", "Torus")),
          _majorRadius(util::ObjectMiddleware::validate<double>(
              args, "majorRadius", "Torus")),
          _minorRadius(util::ObjectMiddleware::validate<double>(
              args, "minorRadius", "Torus")) {
        util::Helpers::unsignedDouble(_majorRadius, "majorRadius", "Torus");
        util::Helpers::unsignedDouble(_minorRadius, "minorRadius", "Torus");
    }

    Torus::Torus(const maths::Vector &center, const double majorRadius,
                 const double minorRadius)
        : Torus(nullptr, center, majorRadius, minorRadius) {
    }

    Torus::Torus(
        std::shared_ptr<raytracer::object::material::IMaterial> material,
        const maths::Vector &center, const double majorRadius,
        const double minorRadius)
        : APrimitive("Torus", center, std::move(material)),
          _majorRadius(majorRadius),
          _minorRadius(minorRadius) {
        util::Helpers::unsignedDouble(_majorRadius, "majorRadius", "Torus");
        util::Helpers::unsignedDouble(_minorRadius, "minorRadius", "Torus");
    }

    const double &Torus::majorRadius() const noexcept {
        return _majorRadius;
    }

    const double &Torus::minorRadius() const noexcept {
        return _minorRadius;
    }

    bool Torus::hits(const maths::Ray &ray, HitRecord &record) const {
        const double mag = ray.direction.magnitude();
        if (mag < 1e-9)
            return false;
        const maths::Vector direction = ray.direction / mag;
        const maths::Vector localOrigin = ray.origin - _center;
        const double R2 = _majorRadius * _majorRadius;
        const double r2 = _minorRadius * _minorRadius;
        const double pd = localOrigin.dot(direction);
        const double pp = localOrigin.dot(localOrigin);
        const double k = pp - R2 - r2;
        constexpr double A4 = 1.0;
        const double a3 = 4.0 * pd;
        const double a2 =
            2.0 * k + 4.0 * pd * pd + 4.0 * R2 * direction.y * direction.y;
        const double a1 = 4.0 * k * pd + 8.0 * R2 * localOrigin.y * direction.y;
        const double a0 =
            k * k + 4.0 * R2 * localOrigin.y * localOrigin.y - 4.0 * R2 * r2;
        std::array<double, 4> roots{};
        const int count =
            maths::PolynomialSolver::solveQuartic(A4, a3, a2, a1, a0, roots);

        if (count <= 0)
            return false;

        auto evalP = [&](const double t) noexcept -> double {
            return ((((A4 * t) + a3) * t + a2) * t + a1) * t + a0;
        };
        auto evalDP = [&](const double t) noexcept -> double {
            return (((4.0 * A4 * t) + 3.0 * a3) * t + 2.0 * a2) * t + a1;
        };
        double tMin = std::numeric_limits<double>::infinity();

        for (int i = 0; i < count; ++i) {
            double ti = roots[i];

            for (int iter = 0; iter < 5; ++iter) {
                const double f = evalP(ti);
                const double df = evalDP(ti);
                if (std::fabs(df) < 1e-15)
                    break;
                ti -= f / df;
            }
            if (ti > 0.01 && ti < tMin) {
                const maths::Vector P = localOrigin + direction * ti;
                const double p2 = P.dot(P);
                const double val = p2 + R2 - r2;
                const double res =
                    val * val - 4.0 * R2 * (P.x * P.x + P.z * P.z);

                if (std::fabs(res) < 0.01 * (4.0 * R2 * r2)) {
                    tMin = ti;
                }
            }
        }
        if (tMin == std::numeric_limits<double>::infinity())
            return false;
        record.t = tMin / mag;
        record.hitPoint = ray.origin + ray.direction * record.t;
        record.objectId = id();
        return true;
    }

    IPrimitive::AABoundingBox Torus::boundingBox() {
        const double rTotal = _majorRadius + _minorRadius + 0.1;
        return maths::AABoundingBox{.x = _center.x - rTotal,
                                    .y = _center.y - _minorRadius - 0.1,
                                    .z = _center.z - rTotal,
                                    .w = rTotal * 2.0,
                                    .h = (_minorRadius + 0.1) * 2.0,
                                    .d = rTotal * 2.0};
    }

    maths::Vector Torus::computeNormal(const maths::Vector &localPoint,
                                       double R) {
        const double k = std::sqrt(localPoint.x * localPoint.x +
                                   localPoint.z * localPoint.z);

        if (k < 1e-10)
            return maths::Vector(0, localPoint.y > 0 ? 1 : -1, 0);
        const double commonFactor = 1.0 - R / k;
        const maths::Vector normal(localPoint.x * commonFactor, localPoint.y,
                                   localPoint.z * commonFactor);

        return normal.normalized();
    }

    SurfaceData Torus::surfaceData(const HitRecord &record) const {
        const maths::Vector localHit = record.hitPoint - _center;
        const maths::Vector normal = computeNormal(localHit, _majorRadius);
        const double u =
            0.5 + std::atan2(localHit.z, localHit.x) / (2.0 * std::numbers::pi);
        const double radialDistance =
            std::sqrt(localHit.x * localHit.x + localHit.z * localHit.z);
        const double v =
            0.5 + std::atan2(localHit.y, radialDistance - _majorRadius) /
                      (2.0 * std::numbers::pi);
        SurfaceData data{
            .normal = normal, .uv = maths::Vector(u, v, 0.0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, record.hitPoint);
        }
        return data;
    }
}  // namespace raytracer::object::primitive