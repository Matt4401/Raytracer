/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#include "math/BVHNode.hpp"

#include "object/primitive/IPrimitive.hpp"

namespace raytracer::maths {
    BVHNode::BVHNode(const BoundingBox &box, std::shared_ptr<IPrimitive> left,
                     std::shared_ptr<IPrimitive> right)
        : _bbox(box), _left(std::move(left)), _right(std::move(right)) {
        _center = computeCenter();
    }

    BVHNode::BVHNode(const BoundingBox &box,
                     std::vector<std::shared_ptr<IPrimitive>> primitives)
        : _bbox(box), _primitives(std::move(primitives)) {
        _center = computeCenter();
    }

    double BVHNode::hits(const Ray &ray) {
        if (isLeaf()) {
            double closestHit = -1.0;
            for (const auto &primitive : _primitives) {
                double hit = primitive->hits(ray);
                if (hit > 0 && (closestHit < 0 || hit < closestHit)) {
                    closestHit = hit;
                }
            }
            return closestHit;
        }

        double leftHit = _left ? _left->hits(ray) : -1.0;
        double rightHit = _right ? _right->hits(ray) : -1.0;

        if (leftHit > 0 && rightHit > 0) {
            return std::min(leftHit, rightHit);
        } else if (leftHit > 0) {
            return leftHit;
        } else {
            return rightHit;
        }
    }

    object::primitive::IPrimitive::BoundingBox BVHNode::boundingBox() {
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