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
        const maths::Vector localOrigin = ray.origin - _center;
        const maths::Vector &direction = ray.direction;

        const double majorRadius2 = _majorRadius * _majorRadius;
        const double minorRadius2 = _minorRadius * _minorRadius;

        const double dd = direction.dot(direction);
        const double pd = localOrigin.dot(direction);
        const double pp = localOrigin.dot(localOrigin);
        const double k = pp + majorRadius2 - minorRadius2;
        const double dxz2 =
            direction.x * direction.x + direction.z * direction.z;
        const double oxdxOzdz =
            localOrigin.x * direction.x + localOrigin.z * direction.z;
        const double oxz2 =
            localOrigin.x * localOrigin.x + localOrigin.z * localOrigin.z;

        const double a4 = dd * dd;
        const double a3 = 4.0 * dd * pd;
        const double a2 =
            2.0 * dd * k + 4.0 * pd * pd - 4.0 * majorRadius2 * dxz2;
        const double a1 = 4.0 * pd * k - 8.0 * majorRadius2 * oxdxOzdz;
        const double a0 = k * k - 4.0 * majorRadius2 * oxz2;

        std::array<double, 4> roots{};
        const int count =
            maths::PolynomialSolver::solveQuartic(a4, a3, a2, a1, a0, roots);

        // ── Évaluation du polynôme et de sa dérivée (Horner) ─────────────────
        auto evalP = [&](double t) noexcept -> double {
            return ((((a4 * t) + a3) * t + a2) * t + a1) * t + a0;
        };
        auto evalDP = [&](double t) noexcept -> double {
            return (((4.0 * a4 * t) + 3.0 * a3) * t + 2.0 * a2) * t + a1;
        };

        // ── Validation géométrique via l'équation implicite du tore ──────────
        // F(P) = (|P|² + R² - r²)² - 4R²(Px²+Pz²)
        // Normalisé par l'échelle (4R²(R+r)²) pour être indépendant de la
        // taille.
        const double Rplusr = _majorRadius + _minorRadius;
        const double torusScale = 4.0 * majorRadius2 * Rplusr * Rplusr;

        auto isValidRoot = [&](double t) noexcept -> bool {
            const double px = localOrigin.x + direction.x * t;
            const double py = localOrigin.y + direction.y * t;
            const double pz = localOrigin.z + direction.z * t;
            const double len2 = px * px + py * py + pz * pz;
            const double f = len2 + majorRadius2 - minorRadius2;
            const double res = f * f - 4.0 * majorRadius2 * (px * px + pz * pz);
            return std::fabs(res) < 0.05 * torusScale;
        };

        double t = std::numeric_limits<double>::infinity();

        for (int i = 0; i < count; ++i) {
            double ti = roots[i];

            // On affine la racine
            for (int iter = 0; iter < 4; ++iter) {
                const double f = evalP(ti);
                const double df = evalDP(ti);
                if (std::fabs(df) < 1e-12) break;
                ti -= f / df;
            }

            // --- CORRECTION ICI ---
            // On augmente le seuil de validation à 0.5 (très permissif)
            // pour boucher tous les trous du Path Tracing
            if (ti > 0.02 && ti < t) {
                if (isValidRoot(ti) || std::fabs(evalP(ti)) < 0.1) {
                    t = ti;
                }
            }
        }

        if (!std::isfinite(t))
            return false;

        record.t = t;
        record.hitPoint = ray.origin + direction * t;
        record.objectId = id();
        return true;
    }

    IPrimitive::AABoundingBox Torus::boundingBox() {
        const double r_total =
            _majorRadius + _minorRadius + 0.1;  // + marge de sécurité
        return maths::AABoundingBox{.x = _center.x - r_total,
                                    .y = _center.y - _minorRadius - 0.1,
                                    .z = _center.z - r_total,
                                    .w = r_total * 2.0,
                                    .h = (_minorRadius + 0.1) * 2.0,
                                    .d = r_total * 2.0};
    }

    maths::Vector Torus::computeNormal(const maths::Vector &localPoint,
                                       double R) {
        // k est la distance du point à l'axe vertical (Y)
        double k = std::sqrt(localPoint.x * localPoint.x +
                             localPoint.z * localPoint.z);

        // On évite la division par zéro si le rayon passe exactement au centre
        if (k < 1e-10)
            return maths::Vector(0, localPoint.y > 0 ? 1 : -1, 0);

        // Calcul de la normale via le gradient de la fonction implicite du tore
        // n = [ 4x(x²+y²+z²+R²-r²) - 8R²x, 4y(x²+y²+z²+R²-r²),
        // 4z(x²+y²+z²+R²-r²) - 8R²z ] Mais on peut simplifier géométriquement
        // pour plus de stabilité :
        double commonFactor = 1.0 - R / k;
        maths::Vector normal(localPoint.x * commonFactor, localPoint.y,
                             localPoint.z * commonFactor);

        return normal.normalized();
    }

    SurfaceData Torus::surfaceData(const HitRecord &record) const {
        // SOUSTRAIRE LE CENTRE ICI pour passer en repère local
        const maths::Vector localHit = record.hitPoint - _center;

        const maths::Vector normal = computeNormal(localHit, _majorRadius);

        // Utiliser localHit aussi pour les UV pour qu'ils suivent l'objet
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