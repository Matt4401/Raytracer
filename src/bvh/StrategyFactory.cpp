/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** StrategyFactory
*/

#include "bvh/StrategyFactory.hpp"

#include "bvh/bvhSplitStrategy/MedianStrategy.hpp"
#include "bvh/bvhSplitStrategy/MidpointStrategy.hpp"
#include "bvh/bvhSplitStrategy/SAHStrategy.hpp"

namespace raytracer::bvh {
    void StrategyFactory::registerStrategy(const std::string& name,
                                           const StrategyCreator& creator) {
        getRegistry()[name] = creator;
    }

    std::unique_ptr<ISplitStrategy> StrategyFactory::create(
        const std::string& name) {
        if (auto& registry = getRegistry(); registry.contains(name)) {
            return registry[name]();
        }
        throw exception::CoreException(
            "StrategyFactory: No strategy registered with name {}", name);
    }

    std::unordered_map<std::string, StrategyCreator>&
    StrategyFactory::getRegistry() {
        static std::unordered_map<std::string, StrategyCreator> registry = {
            {"sah", []() { return std::make_unique<SAHStrategy>(); }},
            {"median", []() { return std::make_unique<MedianStrategy>(); }},
            {"midpoint", []() { return std::make_unique<MidpointStrategy>(); }},
        };

        return registry;
    }
}  // namespace raytracer::bvh
