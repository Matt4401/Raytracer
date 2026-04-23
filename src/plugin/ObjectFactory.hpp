/*
** EPITECH PROJECT, 2026
** ObjectFactory
** File description:
** ObjectFactory header
*/

#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../include/object/IObject.hpp"

namespace raytracer {

    class ObjectFactory {
      public:
        ObjectFactory() = default;
        ~ObjectFactory() = default;

        void registerBuild(const std::string &name,
                           const object::buildFunction &builder);

        std::unique_ptr<object::IObject> build(
            const std::string &name, const std::vector<std::any> &param);

      private:
        std::unordered_map<std::string, object::buildFunction> _builders;
    };

}  // namespace raytracer
