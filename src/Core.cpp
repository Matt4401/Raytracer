/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <map>
#include <memory>

#include "object/IObject.hpp"
#include "parser/ConfigParser.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init(const std::filesystem::path &file) {
        parsing::ConfigParser parser;
        std::vector<std::unique_ptr<object::IObject>> objects;

        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

        parser.setBuildCallback(
            [this](const std::string &name,
                   const std::map<std::string, std::any> &param) {
                return this->_objFactory.build(name, param);
            });
        parser.setAssignCallback(
            [&objects](std::unique_ptr<object::IObject> object) {
                objects.emplace_back(std::move(object));
            });
        parser.parse(file);
    }

    void Core::run() {
        // TODO: main compute
    }

}  // namespace raytracer
