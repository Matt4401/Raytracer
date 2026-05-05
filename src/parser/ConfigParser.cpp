/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser code
*/

#include "parser/ConfigParser.hpp"

#include <any>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingException.hpp"
#include "object/IObject.hpp"
#include "object/IScene.hpp"
#include "parser/LibCfgUtils.hpp"

namespace raytracer::parsing {

    void ConfigParser::setBuildCallback(
        const ConfigParser::buildCallback &callback) {
        this->_buildCallback = callback;
    }

    void ConfigParser::loadConfig(const std::filesystem::path &filepath) {
        try {
            this->_cfg.clear();
            this->_scenes.clear();
            this->_cfg.readFile(filepath);
        } catch (...) {
            throw raytracer::exception::ParsingException(
                "Failed to open \"{}\" file", filepath.string());
        }
    }

    void ConfigParser::buildAndAssign(
        const ObjectInfo &info,
        const std::function<void(std::shared_ptr<object::IObject> &)> &func) {
        std::shared_ptr<object::IObject> object =
            this->_buildCallback(info.name, info.params);

        if (object != nullptr) {
            func(object);
        } else {
            throw raytracer::exception::ParsingException(
                "Failed to build \"{}\"", info.name);
        }
    }

    ConfigParser::ObjectInfo ConfigParser::getObjectInfo(
        const libconfig::Setting &objectName) {
        ObjectInfo info;

        try {
            const libconfig::Setting &params =
                objectName.lookup(std::string(K_PARAMETERS_KEYWORD));

            info.params = LibCfgUtils::groupToMap(params);
            const libconfig::Setting &name =
                objectName.lookup(std::string(K_NAME_KEYWORD));
            if (!name.isString()) {
                throw raytracer::exception::ParsingException(
                    "did not found parameters");
            }
            info.name =
                std::any_cast<std::string>(LibCfgUtils::extractValue(name));
        } catch (...) {
            info.params = LibCfgUtils::groupToMap(objectName);
        }
        return info;
    }

    void ConfigParser::computeObject(const std::string &parentName,
                                     const libconfig::Setting &objectData) {
        ObjectInfo info = this->getObjectInfo(objectData);
        if (info.params.empty()) {
            return;
        }
        if (info.name.empty()) {
            info.name = parentName;
        }
        try {
            const libconfig::Setting &materialConfig =
                objectData.lookup(std::string(K_MATERIAL_KEYWORD));
            ObjectInfo materialInfo = this->getObjectInfo(materialConfig);

            this->buildAndAssign(
                materialInfo,
                [&info](std::shared_ptr<object::IObject> &object) {
                    info.params.emplace(K_MATERIAL_KEYWORD, object);
                });

        } catch (...) {
        }
        this->buildAndAssign(info,
                             [this](std::shared_ptr<object::IObject> &object) {
                                 this->_scenes.at(0)->addObject(object);
                             });
    }

    void ConfigParser::parseObjectList(const libconfig::Setting &list) {
        for (unsigned int index = 0; index < list.getLength(); ++index) {
            this->computeObject(list.getName(), list[index]);
        }
    }

    void ConfigParser::parseGroups(const libconfig::Setting &list) {
        for (auto &value : list) {
            if (value.isList()) {
                this->parseObjectList(value);
                continue;
            }
            this->computeObject(list.getName(), value);
        }
    }

    std::shared_ptr<object::scene::IScene> ConfigParser::makeScene(
        libconfig::Setting &root) {
        try {
            const libconfig::Setting &sceneCfg =
                root.lookup(std::string(K_SCENE_PARAMETERS));
            std::map<std::string, std::any> params =
                LibCfgUtils::groupToMap(sceneCfg);
            std::shared_ptr<raytracer::object::IObject> objectScene =
                this->_buildCallback("scene", params);

            if (objectScene == nullptr)
                return nullptr;
            if (auto scene = std::dynamic_pointer_cast<object::scene::IScene>(
                    objectScene)) {
                root.remove(sceneCfg.getName());
                return scene;
            }
        } catch (...) {
            return nullptr;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<object::scene::IScene>> ConfigParser::parse(
        const std::filesystem::path &filepath) {
        if (this->_buildCallback == nullptr) {
            throw raytracer::exception::ParsingException(
                "Parsing parameter not set");
        }
        this->loadConfig(filepath);
        if (std::shared_ptr<object::scene::IScene> scene =
                this->makeScene(this->_cfg.getRoot())) {
            this->_scenes.emplace_back(scene);
        }
        for (const auto &objectsTheme : this->_cfg.getRoot()) {
            if (objectsTheme.isGroup()) {
                this->parseGroups(objectsTheme);
            }
            if (objectsTheme.isList()) {
                this->parseObjectList(objectsTheme);
            }
        }
        return this->_scenes;
    }
}  // namespace raytracer::parsing
