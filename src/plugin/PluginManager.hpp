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

        /**
         * @brief Populate the factory with builder functions from loaded
         * plugins
         *
         * @param factory: Reference to the ObjectFactory which will be populate
         */
        void fillFactory(ObjectFactory &factory);

        /**
         * @brief Update the list of loaded plugins from a directory
         *
         * Scans the specified directory for plugin libraries and loads them.
         * @param path: Path to the directory containing plugin libraries
         */
        void updatePluginList(const std::filesystem::path &path);

      private:
        std::vector<DlLoader> _pluginLoaderList;

        static constexpr std::string_view ENTRY_NAME = "name";
        static constexpr std::string_view ENTRY_BUILDER = "builder";
        static constexpr std::string_view ENTRY_TYPE = "type";

        /**
         * @brief Get a list of library paths from a directory
         *
         * @param path: The directory path to search
         * @return A vector of filesystem paths to found libraries
         */
        static std::vector<std::filesystem::path> fetchLibsPath(
            const std::filesystem::path &path);

        /**
         * @brief Recursively collect library files from a folder and its
         * subdirectories
         *
         * @param libs: Reference to vector where library paths will be stored
         * @param folder: The folder to search recursively
         */
        static void getLibsFromFolder(std::vector<std::filesystem::path> &libs,
                                      const std::filesystem::path &folder);
    };

}  // namespace raytracer
