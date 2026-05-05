/*
** EPITECH PROJECT, 2026
** LibConfigUtils
** File description:
** LibConfigUtils code
*/

#include "LibCfgUtils.hpp"

#include <iostream>
#include <libconfig.h++>

#include "exception/ParsingException.hpp"

namespace raytracer::parsing {

    std::any LibCfgUtils::extractValue(const libconfig::Setting &setting) {
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

    std::vector<std::any> LibCfgUtils::arrayToVector(
        const libconfig::Setting &array) {
        std::vector<std::any> data;

        for (const auto &value : array) {
            try {
                data.emplace_back(LibCfgUtils::extractValue(value));
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
                // std::cout << value.getName() << "\n";
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

}  // namespace raytracer::parsing
