/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHBuilder
*/

#pragma once

#include <chrono>
#include <memory>
#include <string_view>
#include <vector>

#include "bvh/BVHNode.hpp"
#include "bvh/ISplitStrategy.hpp"
#include "bvh/StrategyFactory.hpp"
#include "math/AABoundingBox.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::bvh {
    using PrimitiveVec =
        std::vector<std::shared_ptr<object::primitive::IPrimitive>>;

    template <typename Strategy>
    class BVHBuilder {
      public:
        explicit BVHBuilder(const std::string_view name) {
            _strategy = StrategyFactory::create(std::string(name));
        }
        ~BVHBuilder() = default;

        static constexpr std::size_t MIN_OBJECTS_PER_LEAF = 2;

        std::shared_ptr<object::primitive::IPrimitive> build(
            const PrimitiveVec& objects) {
            maths::AABoundingBox box = calculateGlobalBox(objects);
            const SplitResult res = _strategy->findSplit(objects, box);

            if (shouldCreateLeaf(objects, res)) {
                return std::make_shared<BVHNode>(box, objects);
            }
            auto [leftObjs, rightObjs] = partition(objects, res);
            if (leftObjs.empty() || rightObjs.empty()) {
                return std::make_shared<BVHNode>(box, objects);
            }
            auto leftChild = build(leftObjs);
            auto rightChild = build(rightObjs);
            return std::make_shared<BVHNode>(box, leftChild, rightChild);
        }

      private:
        std::unique_ptr<Strategy> _strategy;

        static bool shouldCreateLeaf(const PrimitiveVec& objs,
                                     const SplitResult& res) {
            return !res.shouldSplit || objs.size() <= MIN_OBJECTS_PER_LEAF;
        }

        static double getCenterOnAxis(
            const std::shared_ptr<object::primitive::IPrimitive>& obj,
            const Axis axis) {
            const auto [x, y, z, w, h, d] = obj->boundingBox();
            if (axis == Axis::X) {
                return x + w / 2;
            }
            if (axis == Axis::Y) {
                return y + h / 2;
            }
            return z + d / 2;
        }

        static std::pair<PrimitiveVec, PrimitiveVec> partition(
            const PrimitiveVec& objs, const SplitResult& res) {
            PrimitiveVec left;
            PrimitiveVec right;

            for (const auto& obj : objs) {
                if (const double center = getCenterOnAxis(obj, res.axis);
                    center < res.splitPos)
                    left.push_back(obj);
                else
                    right.push_back(obj);
            }
            return {std::move(left), std::move(right)};
        }

        static object::primitive::IPrimitive::AABoundingBox calculateGlobalBox(
            const PrimitiveVec& objs) {
            if (objs.empty()) {
                return {0, 0, 0, 0, 0, 0};
            }
            double minX = std::numeric_limits<double>::infinity();
            double minY = std::numeric_limits<double>::infinity();
            double minZ = std::numeric_limits<double>::infinity();
            double maxX = -std::numeric_limits<double>::infinity();
            double maxY = -std::numeric_limits<double>::infinity();
            double maxZ = -std::numeric_limits<double>::infinity();

            for (const auto& obj : objs) {
                auto [x, y, z, w, h, d] = obj->boundingBox();
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                minZ = std::min(minZ, z);
                maxX = std::max(maxX, x + w);
                maxY = std::max(maxY, y + h);
                maxZ = std::max(maxZ, z + d);
            }
            return {.x = minX,
                    .y = minY,
                    .z = minZ,
                    .w = maxX - minX,
                    .h = maxY - minY,
                    .d = maxZ - minZ};
        }
    };
}  // namespace raytracer::bvh
