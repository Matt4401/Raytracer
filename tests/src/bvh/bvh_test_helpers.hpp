/*
** EPITECH PROJECT, 2026
** bvh_test_helpers
** File description:
** Shared helpers for BVH tests
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bvh/ASplitStrategy.hpp"
#include "bvh/ISplitStrategy.hpp"
#include "math/AABoundingBox.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::tests::bvh {
    class TestPrimitive final : public object::primitive::IPrimitive {
      public:
        TestPrimitive(std::string name, maths::AABoundingBox box,
                      maths::Vector center, double hitT, int id = -1)
            : _name(std::move(name)),
              _box(box),
              _center(center),
              _hitT(hitT),
              _id(id) {}

        bool hits(const maths::Ray &,
                  object::primitive::HitRecord &record) const override {
            if (_hitT < 0.0) {
                return false;
            }
            record.t = _hitT;
            record.objectId = _id;
            return true;
        }

        object::primitive::SurfaceData surfaceData(
            const maths::Vector &) const override {
            return {};
        }

        AABoundingBox boundingBox() override {
            return _box;
        }

        const std::string &name() const noexcept override {
            return _name;
        }

        maths::Vector center() const noexcept override {
            return _center;
        }

        void setId(int id) override {
            _id = id;
        }

        int getId() const override {
            return _id;
        }

      private:
        std::string _name;
        maths::AABoundingBox _box;
        maths::Vector _center;
        double _hitT;
        int _id = -1;
    };

    inline std::shared_ptr<TestPrimitive> makePrimitive(
        const std::string &name, const maths::AABoundingBox &box,
        const maths::Vector &center, double hitT, int id = -1) {
        return std::make_shared<TestPrimitive>(name, box, center, hitT, id);
    }

    class ExposedASplitStrategy : public raytracer::bvh::ASplitStrategy {
      public:
        raytracer::bvh::SplitResult findSplit(
            const std::vector<std::shared_ptr<raytracer::object::primitive::IPrimitive>> &,
            const maths::AABoundingBox &) override {
            return {.axis = raytracer::bvh::Axis::X,
                    .splitPos = 0.0,
                    .shouldSplit = false};
        }

        static raytracer::bvh::Axis longestAxis(const maths::AABoundingBox &box) {
            return raytracer::bvh::ASplitStrategy::longestAxis(box);
        }

        static bool hasEnoughPrimitives(std::size_t count) {
            return raytracer::bvh::ASplitStrategy::hasEnoughPrimitives(count);
        }
    };
}  // namespace raytracer::tests::bvh