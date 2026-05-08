/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** StrategyFactory
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "bvh/ISplitStrategy.hpp"
#include "exception/CoreException.hpp"

namespace raytracer::bvh {
    using StrategyCreator = std::function<std::unique_ptr<ISplitStrategy>()>;

    class StrategyFactory {
      public:
        static void registerStrategy(const std::string& name,
                                     const StrategyCreator& creator);
        static std::unique_ptr<ISplitStrategy> create(const std::string& name);

      private:
        static std::unordered_map<std::string, StrategyCreator>& getRegistry();
    };
}  // namespace bvh
