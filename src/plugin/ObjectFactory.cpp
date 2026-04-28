/*
** EPITECH PROJECT, 2026
** ObjectFactory
** File description:
** ObjectFactory code
*/

#include "ObjectFactory.hpp"

#include <iostream>
#include <memory>

#include "exception/PluginException.hpp"
#include "object/IObject.hpp"

namespace raytracer {

    void ObjectFactory::registerBuild(const std::string &name,
                                      const object::buildFunction &builder) {
        this->_builders.insert_or_assign(name, builder);
    }

    std::unique_ptr<object::IObject> ObjectFactory::build(
        const std::string &name, const std::vector<std::any> &param) {
        const auto iter = this->_builders.find(name);

        if (iter == this->_builders.end()) {
            return nullptr;
        }
        try {
            object::IObject *rawObject = iter->second(param);
            if (rawObject == nullptr) {
                return nullptr;
            }
            return std::unique_ptr<object::IObject>(rawObject);
        } catch (const raytracer::exception::PluginException &err) {
            std::cerr << err.what() << "\n";
            return nullptr;
        }
    }
}  // namespace raytracer
