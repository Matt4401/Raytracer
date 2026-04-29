/*
** EPITECH PROJECT, 2026
** ObjectFactory
** File description:
** ObjectFactory header
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "object/IObject.hpp"

namespace raytracer {

    class ObjectFactory {
      public:
        ObjectFactory() = default;
        ~ObjectFactory() = default;

        /**
         * @brief Register a builder function with name as key
         *
         * @param name: The name/identifier of the object type
         * @param builder: The build function used to creates instances of this
         * type
         */
        void registerBuild(const std::string &name,
                           const object::buildFunction &builder);

        /**
         * @brief Build an object base on name. Need parameter
         *
         * @param name: The name of the object type to build
         * @param param :Parameters to pass to the builder function
         * @return A unique pointer to the created IObject, or nullptr if type
         * is not registered or build fails
         */
        std::unique_ptr<object::IObject> build(
            const std::string &name,
            const std::map<std::string, std::any> &param);

      private:
        std::unordered_map<std::string, object::buildFunction> _builders;
    };

}  // namespace raytracer
