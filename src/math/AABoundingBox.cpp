/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BoundingBox
*/

#include "math/AABoundingBox.hpp"

namespace raytracer::maths {
    void AABoundingBox::extend(const AABoundingBox &other) {
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z) ||
            !std::isfinite(w) || !std::isfinite(h) || !std::isfinite(d)) {
            *this = other;
            return;
        }
        if (!std::isfinite(other.x) || !std::isfinite(other.y) ||
            !std::isfinite(other.z) || !std::isfinite(other.w) ||
            !std::isfinite(other.h) || !std::isfinite(other.d)) {
            return;
        }
        const double minX = std::min(x, other.x);
        const double minY = std::min(y, other.y);
        const double minZ = std::min(z, other.z);
        const double maxX = std::max(x + w, other.x + other.w);
        const double maxY = std::max(y + h, other.y + other.h);
        const double maxZ = std::max(z + d, other.z + other.d);

        x = minX;
        y = minY;
        z = minZ;
        w = maxX - minX;
        h = maxY - minY;
        d = maxZ - minZ;
    }

    double AABoundingBox::surfaceData() const {
        if (w < 0 || h < 0 || d < 0)
            return 0;
        return 2 * (w * h + w * d + h * d);
    }

    double AABoundingBox::intersects(const Ray &ray) const {
        double tMin = -std::numeric_limits<double>::infinity();
        double tMax = std::numeric_limits<double>::infinity();
        const double axes[3][3] = {{ray.origin.x, ray.direction.x, x},
                                   {ray.origin.y, ray.direction.y, y},
                                   {ray.origin.z, ray.direction.z, z}};
        const double dims[3] = {w, h, d};

        for (int i = 0; i < 3; ++i) {
            const double direction = axes[i][1];
            const double min = axes[i][2];
            const double max = axes[i][2] + dims[i];
            if (direction == 0.0) {
                if (axes[i][0] < min || axes[i][0] > max) {
                    return -1.0;
                }
                continue;
            }
            const double invD = 1.0 / direction;
            double t0 = (min - axes[i][0]) * invD;
            double t1 = (max - axes[i][0]) * invD;
            tMin = std::max(tMin, std::min(t0, t1));
            tMax = std::min(tMax, std::max(t0, t1));
            if (tMax < tMin)
                return -1.0;
        }
        if (tMax < 0.0) {
            return -1.0;
        }
        return std::max(0.0, tMin);
    }
}  // namespace raytracer::maths
