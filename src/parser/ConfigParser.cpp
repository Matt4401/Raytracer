/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser code
*/

#include "parser/ConfigParser.hpp"

#include <any>
#include <filesystem>
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
        if (this->_buildCallback == nullptr) {
            throw raytracer::exception::ParsingException(
                "Parsing parameter not set");
        }
        try {
            this->_cfg.clear();
            this->_scene = nullptr;
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

    std::shared_ptr<object::IObject> ConfigParser::computeTransforms(
        const std::shared_ptr<object::IObject> &baseObject,
        const libconfig::Setting &objectData) {
        if (!objectData.exists(std::string(K_TRANSFORMATIONS_KEYWORD))) {
            return baseObject;
        }

        const libconfig::Setting &transformList =
            objectData.lookup(std::string(K_TRANSFORMATIONS_KEYWORD));
        if (!transformList.isList() && !transformList.isArray()) {
            throw exception::ParsingException("'{}' must be a list or array",
                                              K_TRANSFORMATIONS_KEYWORD);
        }

        std::shared_ptr<object::IObject> current = baseObject;

        for (int index = 0; index < transformList.getLength(); ++index) {
            const libconfig::Setting &entry = transformList[index];
            if (!entry.isGroup()) {
                throw exception::ParsingException(
                    "Each '{}' entry must be a group",
                    K_TRANSFORMATIONS_KEYWORD);
            }

            std::map<std::string, std::any> params =
                LibCfgUtils::groupToMap(entry);
            const auto nameIt = params.find(std::string(K_NAME_KEYWORD));
            if (nameIt == params.end()) {
                throw exception::ParsingException(
                    "Each '{}' entry must define '{}'",
                    K_TRANSFORMATIONS_KEYWORD, K_NAME_KEYWORD);
            }

            std::string transformName;
            try {
                transformName = std::any_cast<std::string>(nameIt->second);
            } catch (const std::bad_any_cast &) {
                throw exception::ParsingException(
                    "'{}' must be a string in '{}' entry", K_NAME_KEYWORD,
                    K_TRANSFORMATIONS_KEYWORD);
            }
            params.erase(nameIt);
            params.insert_or_assign("primitive", current);

            std::shared_ptr<object::IObject> transformed =
                this->_buildCallback(transformName, params);
            if (!transformed) {
                throw exception::ParsingException(
                    "Failed to build transform '{}'", transformName);
            }
            current = transformed;
        }

        return current;
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

        std::shared_ptr<object::IObject> object =
            this->_buildCallback(info.name, info.params);
        if (!object) {
            throw raytracer::exception::ParsingException(
                "Failed to build \"{}\"", info.name);
        }

        object = this->computeTransforms(object, objectData);
        this->_scene->addObject(object);
    }

    void ConfigParser::parseObjectList(const libconfig::Setting &list) {
        for (int index = 0; index < list.getLength(); ++index) {
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
        if (!root.exists(std::string(K_SCENE_PARAMETERS)))
            throw raytracer::exception::ParsingException(
                "scene Parameter missing");

        const libconfig::Setting &sceneCfg =
            root.lookup(std::string(K_SCENE_PARAMETERS));
        std::map<std::string, std::any> params =
            LibCfgUtils::groupToMap(sceneCfg);
        std::shared_ptr<raytracer::object::IObject> objectScene =
            this->_buildCallback("scene", params);

        if (objectScene == nullptr)
            throw raytracer::exception::ParsingException(
                "failed to build the scene");
        if (auto scene =
                std::dynamic_pointer_cast<object::scene::IScene>(objectScene)) {
            root.remove(sceneCfg.getName());
            this->_scene = scene;
        }
    }

    void ConfigParser::parseObjects(libconfig::Setting &root,
                                    const std::filesystem::path &path) {
        std::vector<std::string> extrasPath =
            LibCfgUtils::stringAggregateToVector(root, K_EXTRA_KEYWORD);

        for (const auto &objectsTheme : root) {
            if (objectsTheme.isGroup())
                this->parseGroups(objectsTheme);
            if (objectsTheme.isList())
                this->parseObjectList(objectsTheme);
        }
        this->subFileHandling(
            extrasPath, path,
            [this](ConfigParser &parser, const std::filesystem::path &path) {
                parser.parse(this->_scene, path);
            });
    }

    void ConfigParser::subFileHandling(
        const std::vector<std::string> &scenesPath,
        const std::filesystem::path &currentPath,
        const std::function<void(ConfigParser &parser,
                                 const std::filesystem::path &path)> &func) {
        parsing::ConfigParser subParser;

        subParser.setBuildCallback(this->_buildCallback);
        for (const auto &pathName : scenesPath) {
            std::filesystem::path path = currentPath.parent_path() / pathName;
            if (!std::filesystem::exists(path) ||
                !std::filesystem::is_regular_file(path)) {
                throw exception::ParsingException(
                    "subFile \"{}\" is not a valid path, in {} config file",
                    path.string(), currentPath.string());
            }
            func(subParser, path);
        }
    }

    std::shared_ptr<object::scene::IScene> ConfigParser::parse(
        const std::filesystem::path &filepath) {
        this->loadConfig(filepath);
        libconfig::Setting &root = this->_cfg.getRoot();
        this->makeScene(root);

        this->parseObjects(root, filepath);
        if (!this->_scene->haveCamera()) {
            throw exception::ParsingException(
                "Scene in {} do not have any cameras", filepath.string());
        }
        return this->_scene;
    }

    void ConfigParser::parse(
        const std::shared_ptr<object::scene::IScene> &sceneBuffer,
        const std::filesystem::path &filepath) {
        this->loadConfig(filepath);
        this->_scene = sceneBuffer;
        this->parseObjects(this->_cfg.getRoot(), filepath);
    }
}  // namespace raytracer::parsing
