/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ATransformDecorator
*/

#include "object/transformations/ATransformDecorator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>

namespace raytracer::object::primitive {

    bool ATransformDecorator::hits(const maths::Ray &ray,
                                   HitRecord &record) const {
        maths::Ray localRay;
        localRay.origin = _inverse.transformPoint(ray.origin);
        localRay.direction = _inverse.transformVector(ray.direction);

        HitRecord localRecord;
        if (!this->_wrapped->hits(localRay, localRecord)) {
            return false;
        }

        const maths::Vector worldHit =
            _matrix.transformPoint(localRecord.hitPoint);
        const double denom = ray.direction.dot(ray.direction);
        double worldT = 0.0;

        if (denom == 0.0) {
            worldT = 0.0;
        } else {
            worldT = (worldHit - ray.origin).dot(ray.direction) / denom;
        }

        record = localRecord;
        record.t = worldT;
        record.hitPoint = worldHit;
        record.objectId = this->id();
        return true;
    }

    IPrimitive::AABoundingBox ATransformDecorator::boundingBox() {
        auto localBox = this->_wrapped->boundingBox();

        if (!std::isfinite(localBox.x) || !std::isfinite(localBox.y) ||
            !std::isfinite(localBox.z) || !std::isfinite(localBox.w) ||
            !std::isfinite(localBox.h) || !std::isfinite(localBox.d)) {
            return localBox;
        }

        const double minX = localBox.x;
        const double minY = localBox.y;
        const double minZ = localBox.z;
        const double maxX = localBox.x + localBox.w;
        const double maxY = localBox.y + localBox.h;
        const double maxZ = localBox.z + localBox.d;

        std::array<maths::Vector, 8> points = {
            maths::Vector(minX, minY, minZ), maths::Vector(minX, minY, maxZ),
            maths::Vector(minX, maxY, minZ), maths::Vector(minX, maxY, maxZ),
            maths::Vector(maxX, minY, minZ), maths::Vector(maxX, minY, maxZ),
            maths::Vector(maxX, maxY, minZ), maths::Vector(maxX, maxY, maxZ),
        };

        double wxmin = std::numeric_limits<double>::infinity();
        double wymin = std::numeric_limits<double>::infinity();
        double wzmin = std::numeric_limits<double>::infinity();
        double wxmax = -std::numeric_limits<double>::infinity();
        double wymax = -std::numeric_limits<double>::infinity();
        double wzmax = -std::numeric_limits<double>::infinity();

        for (const auto &p : points) {
            const auto tp = _matrix.transformPoint(p);
            wxmin = std::min(wxmin, tp.x);
            wymin = std::min(wymin, tp.y);
            wzmin = std::min(wzmin, tp.z);
            wxmax = std::max(wxmax, tp.x);
            wymax = std::max(wymax, tp.y);
            wzmax = std::max(wzmax, tp.z);
        }

        return {wxmin,         wymin,         wzmin,
                wxmax - wxmin, wymax - wymin, wzmax - wzmin};
    }

    SurfaceData ATransformDecorator::surfaceData(
        const HitRecord &record) const {
        HitRecord localRecord = record;
        localRecord.hitPoint = _inverse.transformPoint(record.hitPoint);

        SurfaceData localData = this->_wrapped->surfaceData(localRecord);
        maths::Vector transformedNormal =
            _inverseTranspose.transformVector(localData.normal);
        localData.normal = transformedNormal.normalized();
        return localData;
    }
}  // namespace raytracer::object::primitive