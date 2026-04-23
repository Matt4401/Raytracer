/*
** EPITECH PROJECT, 2026
** LibHandler
** File description:
** LibHandler header
*/

#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include "DlLoader.hpp"
#include "ObjectFactory.hpp"

namespace raytracer {

#if defined(__APPLE__)
    constexpr std::string_view EXTENSION_NAME = ".dylib";
#elif defined(__linux__)
    static constexpr std::string_view EXTENSION_NAME = ".so";
#else
#error "Unsupported platform"
#endif

    class PluginManager {
      public:
        PluginManager() = default;
        ~PluginManager() = default;

        void fillFactory(ObjectFactory &factory);
        void updatePluginList();

      private:
        std::vector<DlLoader> _pluginLoaderList;

        static constexpr std::string_view ENTRY_NAME = "name";
        static constexpr std::string_view ENTRY_BUILDER = "builder";
        static constexpr std::string_view ENTRY_TYPE = "type";
        static constexpr std::string_view FOLDER_PATH = "./plugins/";

        static std::vector<std::filesystem::path> fetchLibsPath();
        static void getLibsFromFolder(std::vector<std::filesystem::path> &libs,
                                      const std::filesystem::path &folder);
    };

}  // namespace raytracer
