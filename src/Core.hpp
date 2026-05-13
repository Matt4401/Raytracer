/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Render.hpp"
#include "export/IExport.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {
    class Core {
      public:
        Core() = default;
        ~Core() = default;

        /**
         * @brief Initialize the core and plugins
         *
         * Loads plugins from the plugins folder and registers them in the
         * factory.
         */
        void init(
            const std::vector<std::string> &argv,
            const std::filesystem::path &pluginsPath = PLUGINS_FOLDER_PATH);

        /**
         * @brief Run the main loop
         *
         * TODO: Implement the main core program
         */
        void run();

        static std::pair<bool, int> helpMessage(
            const std::vector<std::string> &argv);

      private:
        PluginManager _plugManager;
        ObjectFactory _objFactory;
        Render _renderer;

        std::vector<std::shared_ptr<object::scene::IScene>> _scenes;
        std::unique_ptr<exporter::IExport> _export = nullptr;
        std::filesystem::path _givenFile;

        static constexpr std::string_view PLUGINS_FOLDER_PATH = "./plugins/";
        static constexpr std::string_view EXPORT_FLAG = "-e";
        static constexpr std::string_view HELP_MESSAGE =
            "USAGE: ./raytracer <SCENE_FILE> \n"
            "\tSCENE_FILE: scene configuration\n"
            "\t-e: export mode (ppm)\n";

        void cmdArgsHandling(const std::vector<std::string> &argv);

        void setExportViaFlag(size_t index,
                              const std::vector<std::string> &argv);
    };
}  // namespace raytracer
