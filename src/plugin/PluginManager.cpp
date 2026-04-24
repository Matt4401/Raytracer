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

#include "DlLoader.hpp"
#include "object/IObject.hpp"

namespace raytracer {

    void PluginManager::fillFactory(ObjectFactory &factory) {
        if (this->_pluginLoaderList.empty()) {
            return;
        }
        std::erase_if(this->_pluginLoaderList, [&factory](
                                                   const DlLoader &loader) {
            try {
                const char *rawName = loader.get<const char *>(ENTRY_NAME);
                std::string name = rawName ? rawName : "";
                object::buildFunction builderPtr =
                    loader.get<raytracer::object::BuilderFunc>(ENTRY_BUILDER);

                factory.registerBuild(name, builderPtr);
                return false;
            } catch (...) {
                return true;
            }
        });
    }

    void PluginManager::updatePluginList(const std::filesystem::path &path) {
        this->_pluginLoaderList.clear();
        std::vector<std::filesystem::path> libsPath = this->fetchLibsPath(path);

        for (const auto &lib : libsPath) {
            DlLoader loader;

            if (!loader.open(lib)) {
                continue;
            }
            this->_pluginLoaderList.emplace_back(std::move(loader));
        }
    }

    std::vector<std::filesystem::path> PluginManager::fetchLibsPath(
        const std::filesystem::path &path) {
        if (!std::filesystem::exists(path) ||
            !std::filesystem::is_directory(path)) {
            return {};
        }
        std::vector<std::filesystem::path> filepaths;

        PluginManager::getLibsFromFolder(filepaths, path);

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
