/*
** EPITECH PROJECT, 2026
** test_strategy_factory
** File description:
** Unit tests for StrategyFactory
*/

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "bvh/ISplitStrategy.hpp"
#include "bvh/StrategyFactory.hpp"

namespace {
    class CustomStrategy final : public raytracer::bvh::ISplitStrategy {
      public:
        raytracer::bvh::SplitResult findSplit(
            const std::vector<std::shared_ptr<raytracer::object::primitive::IPrimitive>> &,
            const raytracer::maths::AABoundingBox &) override {
            return {.axis = raytracer::bvh::Axis::Z,
                    .splitPos = 42.0,
                    .shouldSplit = true};
        }
    };
}  // namespace

TEST(STRATEGY_FACTORY, creates_built_in_strategies) {
    auto median = raytracer::bvh::StrategyFactory::create("median");
    auto midpoint = raytracer::bvh::StrategyFactory::create("midpoint");
    auto sah = raytracer::bvh::StrategyFactory::create("sah");

    ASSERT_NE(median, nullptr);
    ASSERT_NE(midpoint, nullptr);
    ASSERT_NE(sah, nullptr);
}

TEST(STRATEGY_FACTORY, registers_and_creates_custom_strategy) {
    raytracer::bvh::StrategyFactory::registerStrategy(
        "unit-test-strategy", []() { return std::make_unique<CustomStrategy>(); });

    auto strategy = raytracer::bvh::StrategyFactory::create("unit-test-strategy");

    ASSERT_NE(strategy, nullptr);
    const auto result = strategy->findSplit({}, {0, 0, 0, 1, 1, 1});
    ASSERT_TRUE(result.shouldSplit);
    ASSERT_EQ(result.axis, raytracer::bvh::Axis::Z);
    ASSERT_DOUBLE_EQ(result.splitPos, 42.0);
}

TEST(STRATEGY_FACTORY, unknown_strategy_throws) {
    ASSERT_THROW(raytracer::bvh::StrategyFactory::create("does-not-exist"),
                 raytracer::exception::CoreException);
}