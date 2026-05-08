/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#include "math/BVHNode.hpp"

#include "object/primitive/IPrimitive.hpp"

namespace raytracer::maths {
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

    double BVHNode::chooseNodeHits(const Ray &ray) const {
        const double tMinLeft =
            _left ? _left->boundingBox().intersects(ray) : -1.0;
        const double tMinRight =
            _right ? _right->boundingBox().intersects(ray) : -1.0;
        const auto first =
            (tMinLeft <= tMinRight && tMinLeft > 0) ? _left : _right;
        const auto second = (first == _left) ? _right : _left;
        const double hit1 = first ? first->hits(ray) : -1.0;

        if (const double tMinSecond = (second == _left) ? tMinLeft : tMinRight;
            hit1 > 0 && (tMinSecond < 0 || hit1 < tMinSecond)) {
            return hit1;
        }
        const double hit2 = second ? second->hits(ray) : -1.0;
        if (hit1 > 0 && hit2 > 0)
            return std::min(hit1, hit2);
        return (hit1 > 0) ? hit1 : hit2;
    }

    double BVHNode::hits(const Ray &ray) {
        if (!_bbox.intersects(ray)) {
            return -1.0;
        }

        if (isLeaf()) {
            double closestHit = -1.0;
            for (const auto &primitive : _primitives) {
                if (const double hit = primitive->hits(ray);
                    hit > 0 && (closestHit < 0 || hit < closestHit)) {
                    closestHit = hit;
                }
            }
            return closestHit;
        }
        return chooseNodeHits(ray);
    }

    object::primitive::IPrimitive::AABoundingBox BVHNode::boundingBox() {
        return _bbox;
    }

    object::primitive::SurfaceData BVHNode::surfaceData(
        const maths::Vector &hitPoint) const {
        throw std::runtime_error("BVHNode has no surface data");
    }

    const std::string &BVHNode::name() const noexcept {
        return _name;
    }
    maths::Vector BVHNode::center() const noexcept {
        return _center;
    }

    bool BVHNode::isLeaf() const {
        return _left == nullptr;
    }

    maths::Vector BVHNode::computeCenter() const {
        return Vector(_bbox.x + _bbox.w / 2, _bbox.y + _bbox.h / 2,
                      _bbox.z + _bbox.d / 2);
    }
}  // namespace raytracer::maths