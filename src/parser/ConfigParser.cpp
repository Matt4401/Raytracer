/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser code
*/

#include "parser/ConfigParser.hpp"

#include <any>
#include <exception>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingException.hpp"
#include "exception/PluginException.hpp"
#include "object/IObject.hpp"
#include "object/IScene.hpp"
#include "object/material/IMaterial.hpp"
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

        } catch (const libconfig::FileIOException &) {
            throw raytracer::exception::ParsingException(
                "Failed to open \"{}\" file.", filepath.string());
        } catch (const libconfig::ParseException &e) {
            throw raytracer::exception::ParsingException(
                "Failed to parse \"{}\" file (line {}, {}).", filepath.string(),
                e.getLine(), e.getError());
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
        const bool hasName = objectName.exists(std::string(K_NAME_KEYWORD));
        const bool hasParameters =
            objectName.exists(std::string(K_PARAMETERS_KEYWORD));

        if (!hasName && !hasParameters) {
            info.params = LibCfgUtils::groupToMap(objectName);
            return info;
        }
        if (!hasName || !hasParameters) {
            throw raytracer::exception::ParsingException(
                "Object must define both \"{}\" and \"{}\"", K_NAME_KEYWORD,
                K_PARAMETERS_KEYWORD);
        }
        const libconfig::Setting &params =
            objectName.lookup(std::string(K_PARAMETERS_KEYWORD));
        info.params = LibCfgUtils::groupToMap(params);
        const libconfig::Setting &name =
            objectName.lookup(std::string(K_NAME_KEYWORD));
        if (!name.isString()) {
            throw raytracer::exception::ParsingException(
                "\"{}\" must be a string", K_NAME_KEYWORD);
        }
        info.name = std::any_cast<std::string>(LibCfgUtils::extractValue(name));

        return info;
    }

    void ConfigParser::computeMaterial(ObjectInfo &info,
                                       const libconfig::Setting &objectData) {
        const libconfig::Setting &materialConfig =
            objectData.lookup(std::string(K_MATERIAL_KEYWORD));
        ObjectInfo materialInfo = this->getObjectInfo(materialConfig);

        this->buildAndAssign(
            materialInfo, [&info](std::shared_ptr<object::IObject> &object) {
                auto castedObj =
                    std::dynamic_pointer_cast<object::material::IMaterial>(
                        object);

                if (!castedObj) {
                    throw exception::PluginException("Invalid material object");
                }
                info.params.emplace(K_MATERIAL_KEYWORD, castedObj);
            });
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

        if (objectData.exists(std::string(K_MATERIAL_KEYWORD))) {
            this->computeMaterial(info, objectData);
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

    void ConfigParser::makeScene(libconfig::Setting &root) {
        if (!root.exists(std::string(K_SCENE_PARAMETERS))) {
            throw raytracer::exception::ParsingException(
                "failed to make the scene");
        }

        const libconfig::Setting &sceneCfg =
            root.lookup(std::string(K_SCENE_PARAMETERS));
        std::map<std::string, std::any> params =
            LibCfgUtils::groupToMap(sceneCfg);
        std::shared_ptr<raytracer::object::IObject> objectScene =
            this->_buildCallback("scene", params);

        if (objectScene == nullptr) {
            throw raytracer::exception::ParsingException(
                "failed to make the scene");
        }
        if (auto scene =
                std::dynamic_pointer_cast<object::scene::IScene>(objectScene)) {
            root.remove(sceneCfg.getName());
            this->_scenes.emplace_back(scene);
        }
    }

    std::vector<std::shared_ptr<object::scene::IScene>> ConfigParser::parse(
        const std::filesystem::path &filepath) {
        if (this->_buildCallback == nullptr) {
            throw raytracer::exception::ParsingException(
                "Parsing parameter not set");
        }

        this->loadConfig(filepath);
        this->makeScene(this->_cfg.getRoot());

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
