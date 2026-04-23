/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <iostream>

#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init() {
        this->_plugManager.updatePluginList();
        this->_plugManager.fillFactory(this->_objFactory);
    }

    void Core::run() {
        if (this->_objFactory.build("test", {}) == nullptr) {
            std::cout << "it seem to work\n";
        }
    }

}  // namespace raytracer
