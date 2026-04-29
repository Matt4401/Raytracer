/*
** EPITECH PROJECT, 2026
** ConfigParser
** File description:
** ConfigParser code
*/

#include "parser/ConfigParser.hpp"

#include <any>
#include <exception>
#include <iostream>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingException.hpp"
#include "object/IObject.hpp"

namespace raytracer::parsing {

    void ConfigParser::setAssignCallback(
        const ConfigParser::AssignCallback &callback) {
        this->_assignCallback = callback;
    }

    void ConfigParser::setBuildCallback(
        const ConfigParser::buildCallback &callback) {
        this->_buildCallback = callback;
    }

    void ConfigParser::loadConfig(const std::filesystem::path &filepath) {
        try {
            this->_cfg.readFile(filepath);
        } catch (...) {
            throw raytracer::exception::ParsingException(
                "Failed to open \"{}\" file", filepath.string());
        }
    }

    std::any ConfigParser::extractValue(const libconfig::Setting &setting) {
        switch (setting.getType()) {
            case libconfig::Setting::TypeInt:
                return (int)setting;
            case libconfig::Setting::TypeInt64:
                return ((long long)setting);
            case libconfig::Setting::TypeFloat:
                return ((double)setting);
            case libconfig::Setting::TypeString:
                return ((std::string)setting);
            case libconfig::Setting::TypeBoolean:
                return ((bool)setting);
            default:
                throw raytracer::exception::ParsingException(
                    "Invalid config syntaxe on {}.", setting.getName());
        }
    }
    std::any ConfigParser::resolveValue(const libconfig::Setting &value) {
        if (value.isGroup())
            return parseGroup(value);
        if (value.isArray())
            return parseArray(value);
        return extractValue(value);
    }

    std::vector<std::any> ConfigParser::parseArray(
        const libconfig::Setting &array) {
        std::vector<std::any> data;

        for (const auto &value : array) {
            try {
                data.emplace_back(ConfigParser::extractValue(value));
            } catch (const std::exception &err) {
                std::cerr << err.what() << "\n";
            }
        }

        return data;
    }

    std::map<std::string, std::any> ConfigParser::parseGroup(
        const libconfig::Setting &group) {
        std::map<std::string, std::any> data;

        for (const auto &value : group) {
            try {
                data.emplace(value.getName(), resolveValue(value));
            } catch (const std::exception &err) {
                std::cerr << err.what() << "\n";
            }
        }
        return data;
    }

    void ConfigParser::buildAndAssign(
        const std::string &stringName,
        const std::map<std::string, std::any> &config) {
        std::unique_ptr<object::IObject> object =
            this->_buildCallback(stringName, config);

        if (object != nullptr) {
            this->_assignCallback(std::move(object));
        } else {
            throw raytracer::exception::ParsingException(
                "Failed to build \"{}\"", stringName);
        }
    }

    void ConfigParser::parseObjectList(const std::string &stringName,
                                       const libconfig::Setting &list) {
        for (unsigned int index = 0; index < list.getLength(); ++index) {
            std::map<std::string, std::any> config =
                ConfigParser::parseGroup(list[index]);
            if (config.empty()) {
                continue;
            }
            this->buildAndAssign(stringName, config);
        }
    }

    void ConfigParser::computeUniqueObjects(
        const std::map<std::string, std::any> &data,
        const std::string &typeName) {
        for (const auto &object : data) {
            std::map<std::string, std::any> config;
            try {
                config = std::any_cast<std::map<std::string, std::any>>(
                    object.second);
            } catch (const std::bad_any_cast &err) {
                throw raytracer::exception::ParsingException(
                    "Failed to parse file because of  \"{}\" in {}",
                    object.first, typeName);
            }
            this->buildAndAssign(object.first, config);
        }
    }

    void ConfigParser::parseGroups(const libconfig::Setting &list) {
        std::map<std::string, std::any> buffer;
        bool isObject = true;

        for (auto &value : list) {
            if (value.isList()) {
                this->parseObjectList(value.getName(), value);
                isObject = false;
                continue;
            }
            buffer.emplace(value.getName(), ConfigParser::resolveValue(value));
        }

        if (isObject) {
            this->buildAndAssign(list.getName(), buffer);
        } else {
            this->computeUniqueObjects(buffer, list.getName());
        }
    }

    void ConfigParser::parse(const std::filesystem::path &filepath) {
        if (this->_assignCallback == nullptr ||
            this->_buildCallback == nullptr) {
            throw raytracer::exception::ParsingException(
                "Parsing parameter not set");
        }
        this->_cfg.clear();
        this->loadConfig(filepath);
        for (const auto &objectsTheme : this->_cfg.getRoot()) {
            if (objectsTheme.isGroup()) {
                this->parseGroups(objectsTheme);
            }
        }
    }
}  // namespace raytracer::parsing
