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
        /**
         * Registers a new split strategy with the factory. This function allows
         * you to associate a unique name with a creator function that can
         * instantiate the corresponding split strategy when requested.
         * @param name the name of the strategy to register. This name will be
         * used as a key to identify and create instances of the strategy later
         * on. It should be a unique identifier that represents the specific
         * split strategy being registered.
         * @param creator a function that creates and returns a unique pointer
         * to an instance of the ISplitStrategy. This function is responsible
         * for constructing the specific split strategy object when requested.
         * The creator function should encapsulate the logic for instantiating
         * the strategy, including any necessary parameters or configurations.
         * When the create function of the factory is called with the
         * corresponding name, it will invoke this creator function to generate
         * a new instance of the desired split strategy.
         */
        static void registerStrategy(const std::string& name,
                                     const StrategyCreator& creator);

        /**
         * Creates an instance of a split strategy based on the provided name.
         * This function looks up the name in the factory's registry of
         * strategies and, if found, invokes the associated creator function to
         * create and return an instance of the requested split strategy.
         * @param name the name of the strategy to create. This name should
         * correspond to a strategy that has been previously registered with the
         * factory using the registerStrategy function.
         * @return a unique pointer to an instance of the ISplitStrategy
         * corresponding to the provided name. This instance will be created
         * using the creator function associated with the name in the factory's
         * registry.
         * @throws CoreException if the provided name does not correspond to any
         * registered strategy in the factory. This exception indicates that the
         * requested strategy could not be found in the factory's registry.
         */
        static std::unique_ptr<ISplitStrategy> create(const std::string& name);

      private:
        static std::unordered_map<std::string, StrategyCreator>& getRegistry();
    };
}  // namespace raytracer::bvh
