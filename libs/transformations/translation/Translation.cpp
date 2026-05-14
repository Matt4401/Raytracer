/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Translation primitive decorator
*/

#include "Translation.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <map>
#include <memory>

#include "exception/PluginException.hpp"
#include "object/IObject.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    Translation::Translation(const std::map<std::string, std::any> &params)
        : APrimitiveDecorator(nullptr), AObject(Type::TRANSFORM) {
        const auto objectPtr =
            util::ObjectMiddleware::validate<std::shared_ptr<object::IObject>>(
                params, "primitive", "Translation");
        _wrapped = std::dynamic_pointer_cast<IPrimitive>(objectPtr);
        if (!_wrapped) {
            throw exception::PluginException(
                "Translation requires a primitive object");
        }

        const double x = util::ObjectMiddleware::validate<double>(
            params, "x", "Translation");
        const double y = util::ObjectMiddleware::validate<double>(
            params, "y", "Translation");
        const double z = util::ObjectMiddleware::validate<double>(
            params, "z", "Translation");
        const maths::Vector translationVec(x, y, z);

        _matrix = maths::Matrix4::translation(translationVec);
        _inverse = _matrix.inverse();
    }

    const std::string &Translation::name() const noexcept {
        return _wrapped->name();
    }

    maths::Vector Translation::center() const noexcept {
        return _wrapped->center();
    }

    bool Translation::hits(const maths::Ray &ray, HitRecord &record) const {
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

    Translation::AABoundingBox Translation::boundingBox() {
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

    SurfaceData Translation::surfaceData(const HitRecord &record) const {
        HitRecord localRecord = record;
        localRecord.hitPoint = _inverse.transformPoint(record.hitPoint);

        SurfaceData localData = this->_wrapped->surfaceData(localRecord);

        localData.normal = _matrix.transformNormal(localData.normal);
        return localData;
    }
}  // namespace raytracer::object::primitive
