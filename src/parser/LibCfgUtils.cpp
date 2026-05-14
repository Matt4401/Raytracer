/*
** EPITECH PROJECT, 2026
** LibConfigUtils
** File description:
** LibConfigUtils code
*/

#include "LibCfgUtils.hpp"

#include <iostream>
#include <libconfig.h++>
#include <string>
#include <string_view>

#include "exception/ParsingException.hpp"

namespace raytracer::parsing {

    std::any LibCfgUtils::extractValue(const libconfig::Setting &setting) {
        switch (setting.getType()) {
            case libconfig::Setting::TypeInt:
                return ((int)setting);
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
                    "Invalid config syntax on {}.", setting.getName());
        }
    }

    std::vector<std::any> LibCfgUtils::arrayToVector(
        const libconfig::Setting &array) {
        std::vector<std::any> data;

        for (const auto &value : array) {
            try {
                data.emplace_back(LibCfgUtils::resolveValue(value));
            } catch (const std::exception &err) {
                std::cerr << "Warning: Failed to extract the value of "
                          << value.getName() << "in " << array.getName()
                          << "\n";
            }
        }

        return data;
    }

    std::map<std::string, std::any> LibCfgUtils::groupToMap(
        const libconfig::Setting &group) {
        std::map<std::string, std::any> data;

        for (const auto &value : group) {
            try {
                data.emplace(value.getName(), resolveValue(value));
            } catch (const std::exception &err) {
                std::cerr << "Warning: Failed to resolve the value of "
                          << value.getName() << "in " << group.getName()
                          << "\n";
            }
        }
        return data;
    }

    std::any LibCfgUtils::resolveValue(const libconfig::Setting &value) {
        if (value.isGroup()) {
            return groupToMap(value);
        }
        if (value.isArray())
            return arrayToVector(value);
        return extractValue(value);
    }

    std::vector<std::string> LibCfgUtils::stringAggregateToVector(
        libconfig::Setting &root, const std::string_view &aggregateName) {
        std::string name = std::string(aggregateName);

        if (!root.exists(name)) {
            return {};
        }
        const libconfig::Setting &scenes = root.lookup(name);
        if (!scenes.isList() && !scenes.isArray()) {
            throw exception::ParsingException(
                "{} config must be either a array [] or a list ().",
                aggregateName);
        }
        std::vector<std::string> paths;
        for (const auto &path : scenes) {
            if (!path.isString()) {
                throw raytracer::exception::ParsingException(
                    "sub scenes path {}, is not a string", path.getName());
            }
            paths.emplace_back(path);
        }
        root.remove(name);
        return paths;
    }

}  // namespace raytracer::parsing
