/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <iostream>
#include <string>

#include "parser/ConfigParser.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init(const std::filesystem::path &file) {
        parsing::ConfigParser parser;

        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

    void Core::run() {
        this->_renderer.render(*(this->_scenes.at(0)), 1, 50);
        this->_renderer.pixelToPPM(*(this->_scenes.at(0)));
    }

    std::pair<bool, int> Core::helpMessage(int argc, char **argv) {
        std::string message =
            "USAGE: ./raytracer <SCENE_FILE> \n"
            "\tSCENE_FILE: scene configuration\n";

        if (argc != 2) {
            std::cerr << message;
            return {true, 84};

        }
        std::string parameter = argv[1];
        if (parameter == "-h" || parameter == "--help") {
            std::cout << message;
            return {true, 0};
        }
        return {false, 0};
    }

}  // namespace raytracer
