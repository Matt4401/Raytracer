/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#pragma once

#include <memory>
#include <vector>

#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    class BVHNode : public object::primitive::IPrimitive {
      public:
        BVHNode(const AABoundingBox &box, std::shared_ptr<IPrimitive> left,
                std::shared_ptr<IPrimitive> right);
        BVHNode(const AABoundingBox &box,
                std::vector<std::shared_ptr<IPrimitive>> primitives);
        ~BVHNode() override = default;

        bool hits(const maths::Ray &ray,
                  object::primitive::HitRecord &hitRecord) const override;
        double hits(const maths::Ray &ray) const override;
        AABoundingBox boundingBox() override;

        object::primitive::SurfaceData surfaceData(
            const maths::Vector &hitPoint) const override;
        const std::string &name() const noexcept override;
        maths::Vector center() const noexcept override;

        // Unused here but mandatory to impl
        void setId(int id) override;
        int id() const override;
        void setLimitBox(const AABoundingBox &box) override;
        bool isInfinite() const override;

      private:
        AABoundingBox _bbox;
        std::string _name = "BVHNode";
        maths::Vector _center;
        int _id = -1;

        std::shared_ptr<IPrimitive> _left = nullptr;
        std::shared_ptr<IPrimitive> _right = nullptr;
        std::vector<std::shared_ptr<object::primitive::IPrimitive>> _primitives;

        bool isLeaf() const;
        maths::Vector computeCenter() const;
    };
}  // namespace raytracer::bvh
