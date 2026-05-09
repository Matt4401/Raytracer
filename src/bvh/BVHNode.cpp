/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#include "bvh/BVHNode.hpp"

#include <algorithm>
#include <vector>
#include <array>

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

    // bool BVHNode::hitLeaf(const maths::Ray &ray,
    //                       object::primitive::HitRecord &rec) const {
    //     bool hitAnything = false;
    //     for (const auto &primitive : _primitives) {
    //         if (object::primitive::HitRecord currentHit;
    //             primitive->hits(ray, currentHit)) {
    //             if (!hitAnything || currentHit.t < rec.t) {
    //                 rec = currentHit;
    //                 rec.primitive = primitive;
    //                 hitAnything = true;
    //             }
    //         }
    //     }
    //     return hitAnything;
    // }
    //
    // double BVHNode::hits(const maths::Ray &ray) {
    //     object::primitive::HitRecord rec;
    //     if (!hits(ray, rec)) {
    //         return -1.0;
    //     }
    //     return rec.t;
    // }
    //
    // bool BVHNode::hits(const maths::Ray &ray,
    //                    object::primitive::HitRecord &rec) const {
    //     if (_bbox.intersects(ray) < 0.0) {
    //         return false;
    //     }
    //
    //     if (isLeaf()) {
    //         return hitLeaf(ray, rec);
    //     }
    //
    //     const double leftBoxT = _left ? _left->boundingBox().intersects(ray)
    //                                   : -1.0;
    //     const double rightBoxT = _right ? _right->boundingBox().intersects(ray)
    //                                     : -1.0;
    //     const std::array<std::shared_ptr<IPrimitive>, 2> children = {_left, _right};
    //     const std::array<double, 2> boxTs = {leftBoxT, rightBoxT};
    //     int firstIdx = 0;
    //     int secondIdx = 1;
    //
    //     if (rightBoxT >= 0.0 && (leftBoxT < 0.0 || rightBoxT < leftBoxT)) {
    //         firstIdx = 1;
    //         secondIdx = 0;
    //     }
    //
    //     if (object::primitive::HitRecord firstRec;
    //         children[firstIdx] && boxTs[firstIdx] >= 0.0 &&
    //             children[firstIdx]->hits(ray, firstRec)) {
    //         firstRec.primitive = children[firstIdx];
    //         if (boxTs[secondIdx] < 0.0 || firstRec.t <= boxTs[secondIdx]) {
    //             rec = firstRec;
    //             return true;
    //         }
    //         if (object::primitive::HitRecord secondRec;
    //             children[secondIdx] &&
    //             children[secondIdx]->hits(ray, secondRec)) {
    //             secondRec.primitive = children[secondIdx];
    //             if (secondRec.t < firstRec.t) {
    //                 rec = secondRec;
    //             } else {
    //                 rec = firstRec;
    //             }
    //             return true;
    //         }
    //
    //         rec = firstRec;
    //         return true;
    //     }
    //     if (object::primitive::HitRecord secondRec;
    //         children[secondIdx] && boxTs[secondIdx] >= 0.0 &&
    //             children[secondIdx]->hits(ray, secondRec)) {
    //         secondRec.primitive = children[secondIdx];
    //         rec = secondRec;
    //         return true;
    //     }
    //     return false;
    // }

    // This function will not stay, she is juste here for avoid BVHNode to be abstract
    double BVHNode::hits(const maths::Ray &ray) {
        return -1.0;
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
