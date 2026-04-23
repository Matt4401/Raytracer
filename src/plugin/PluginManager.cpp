/*
** EPITECH PROJECT, 2026
** LibHandler
** File description:
** LibHandler code
*/

#include "PluginManager.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include "../../include/object/IObject.hpp"
#include "DlLoader.hpp"

namespace raytracer {

    void PluginManager::fillFactory(ObjectFactory &factory) {
        std::erase_if(
            this->_pluginLoaderList, [&factory](const DlLoader &loader) {
                try {
                    std::string name = loader.get<std::string>(ENTRY_NAME);
                    object::buildFunction builderPtr =
                        loader.get<object::buildFunction>(ENTRY_BUILDER);

                    factory.registerBuild(name, builderPtr);
                    return false;
                } catch (...) {
                    return true;
                }
            });
    }

    void PluginManager::updatePluginList() {
        this->_pluginLoaderList.clear();
        std::vector<std::filesystem::path> libsPath = this->fetchLibsPath();

        for (const auto &lib : libsPath) {
            DlLoader loader;

            if (!loader.open(lib)) {
                continue;
            }
            this->_pluginLoaderList.emplace_back(std::move(loader));
        }
    }

    std::vector<std::filesystem::path> PluginManager::fetchLibsPath() {
        if (!std::filesystem::exists(FOLDER_PATH) ||
            !std::filesystem::is_directory(FOLDER_PATH)) {
            return {};
        }
        std::vector<std::filesystem::path> filepaths;

        PluginManager::getLibsFromFolder(filepaths, FOLDER_PATH);

        return filepaths;
    }

    void PluginManager::getLibsFromFolder(
        std::vector<std::filesystem::path> &libs,
        const std::filesystem::path &folder) {
        for (const auto &dirEntry :
             std::filesystem::recursive_directory_iterator(
                 folder,
                 std::filesystem::directory_options::skip_permission_denied)) {
            if (dirEntry.is_directory()) {
                PluginManager::getLibsFromFolder(libs, dirEntry.path());
            }
            if (dirEntry.is_regular_file() &&
                dirEntry.path().extension() == EXTENSION_NAME) {
                libs.emplace_back(dirEntry.path());
            }
        }
    }

}  // namespace raytracer
