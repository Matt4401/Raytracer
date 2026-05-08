/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** StrategyFactory
*/

#include "bvh/StrategyFactory.hpp"

namespace bvh {
    void StrategyFactory::registerStrategy(const std::string& name,
                                           const StrategyCreator& creator) {
        getRegistry()[name] = creator;
    }

    std::unique_ptr<ISplitStrategy> StrategyFactory::create(
        const std::string& name) {
        if (auto& registry = getRegistry(); registry.contains(name)) {
            return registry[name]();
        }
        throw raytracer::exception::CoreException(
            "StrategyFactory: No strategy registered with name {}", name);
    }

    std::unordered_map<std::string, StrategyCreator>&
    StrategyFactory::getRegistry() {
        static std::unordered_map<std::string, StrategyCreator> registry;

        return registry;
    }
}  // namespace bvh