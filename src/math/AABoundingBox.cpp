/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BoundingBox
*/

#include "math/AABoundingBox.hpp"

namespace raytracer::maths {
    void AABoundingBox::extend(const AABoundingBox &other) {
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

    bool AABoundingBox::intersects(const Ray &ray) const {
        double tMin = -std::numeric_limits<double>::infinity();
        double tMax = std::numeric_limits<double>::infinity();
        const double axes[3][3] = {{ray.origin.x, ray.direction.x, x},
                                   {ray.origin.y, ray.direction.y, y},
                                   {ray.origin.z, ray.direction.z, z}};
        const double dims[3] = {w, h, d};

        for (int i = 0; i < 3; ++i) {
            const double invD = 1.0 / axes[i][1];
            double t0 = (axes[i][2] - axes[i][0]) * invD;
            double t1 = (axes[i][2] + dims[i] - axes[i][0]) * invD;

            if (invD < 0.0)
                std::swap(t0, t1);
            tMin = std::max(tMin, t0);
            tMax = std::min(tMax, t1);
            if (tMax <= tMin)
                return false;
        }
        return tMax >= 0.0;
    }
}  // namespace raytracer::maths
