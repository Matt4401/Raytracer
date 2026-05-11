/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#include "bvh/BVHNode.hpp"

#include <algorithm>
#include <limits>
#include <vector>

#include "exception/CoreException.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    BVHNode::BVHNode(const AABoundingBox &box, std::shared_ptr<IPrimitive> left,
                     std::shared_ptr<IPrimitive> right)
        : _bbox(box), _left(std::move(left)), _right(std::move(right)) {
        _center = computeCenter();
    }

    BVHNode::BVHNode(const AABoundingBox &box,
                     std::vector<std::shared_ptr<IPrimitive>> primitives)
        : _bbox(box), _primitives(std::move(primitives)) {
        _center = computeCenter();
    }

    bool BVHNode::hits(const maths::Ray &ray,
                       object::primitive::HitRecord &hitRecord) const {
        hitRecord = {};

        if (_bbox.intersects(ray) < 0.0) {
            return false;
        }
        if (isLeaf()) {
            bool hitAnything = false;
            double closestSoFar = std::numeric_limits<double>::infinity();

            for (const auto &primitive : _primitives) {
                if (!primitive) {
                    continue;
                }
                if (object::primitive::HitRecord tempRecord;
                    primitive->hits(ray, tempRecord) && tempRecord.t < closestSoFar) {
                    closestSoFar = tempRecord.t;
                    hitRecord = std::move(tempRecord);
                    hitAnything = true;
                }
            }
            return hitAnything;
        }

        const double tLeft =
            _left ? _left->boundingBox().intersects(ray) : -1.0;
        const double tRight =
            _right ? _right->boundingBox().intersects(ray) : -1.0;
        const std::shared_ptr<IPrimitive> first =
            (tLeft <= tRight && tLeft >= 0.0) ? _left : _right;
        const std::shared_ptr<IPrimitive> second = (first == _left) ? _right : _left;
        const double tFirst = (first == _left) ? tLeft : tRight;
        const double tSecond = (first == _left) ? tRight : tLeft;
        bool hitAny = false;

        if (tFirst >= 0.0 && first) {
            hitAny = first->hits(ray, hitRecord);
        }
        if (tSecond >= 0.0 && second && (!hitAny || tSecond < hitRecord.t)) {
            if (object::primitive::HitRecord secondRecord;
                second->hits(ray, secondRecord) &&
                (!hitAny || secondRecord.t < hitRecord.t)) {
                hitRecord = std::move(secondRecord);
                hitAny = true;
            }
        }
        return hitAny;
    }

    double BVHNode::hits(const maths::Ray &ray) const {
        object::primitive::HitRecord record;
        return hits(ray, record) ? record.t : -1.0;
    }

    object::primitive::IPrimitive::AABoundingBox BVHNode::boundingBox() {
        return _bbox;
    }

    object::primitive::SurfaceData BVHNode::surfaceData(
        const maths::Vector &hitPoint) const {
        throw exception::CoreException(
            "BVHNode::surfaceData should not be called on a BVHNode");
    }

    const std::string &BVHNode::name() const noexcept {
        return _name;
    }
    maths::Vector BVHNode::center() const noexcept {
        return _center;
    }

    bool BVHNode::isLeaf() const {
        return !_left && !_right;
    }

    maths::Vector BVHNode::computeCenter() const {
        return maths::Vector(_bbox.x + _bbox.w / 2, _bbox.y + _bbox.h / 2,
                             _bbox.z + _bbox.d / 2);
    }
}  // namespace raytracer::bvh
