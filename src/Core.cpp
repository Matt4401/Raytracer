/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init() {
        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);
    }

    void Core::run() {
        // TODO: main compute
    }

}  // namespace raytracer
