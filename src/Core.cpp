/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include "parser/ConfigParser.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init(const std::filesystem::path &file) {
        parsing::ConfigParser parser;

        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

        parser.setBuildCallback(
            [this](const std::string &name,
                   const std::map<std::string, std::any> &param) {
                return this->_objFactory.build(name, param);
            });
        this->_scenes = parser.parse(file);
    }

    void Core::run() {
        this->_renderer.render(*(this->_scenes.at(0)), 1, 50);
        this->_renderer.pixelToPPM(*(this->_scenes.at(0)));
    }

}  // namespace raytracer
