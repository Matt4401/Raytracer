/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Render.hpp"
#include "exporter/IExport.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"
#include "visual/IVisual.hpp"

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
        size_t _selectedScene = 0;

        std::map<std::filesystem::path, std::shared_ptr<object::scene::IScene>>
            _scenes;
        std::unique_ptr<exporter::IExport> _export = nullptr;
        std::unique_ptr<visual::IVisual> _visual = nullptr;

        static constexpr std::string_view PLUGINS_FOLDER_PATH = "./plugins/";
        static constexpr std::string_view EXPORT_FLAG = "-e";
        static constexpr std::string_view VISUAL_FLAG = "-v";
        static constexpr int PREVIEW_SAMPLE_PER_PIXEL = 10;
        static constexpr std::string_view HELP_MESSAGE =
            "USAGE: ./raytracer <SCENE_FILE> \n"
            "\tSCENE_FILE: scene configuration\n"
            "\t-e: export mode (ppm)\n"
            "\t-v: visual mode (cli/sfml)\n";

        void runScene(const std::shared_ptr<object::scene::IScene> &scene);

        template <typename Base>
        using Factory = std::function<std::unique_ptr<Base>()>;
        void cmdArgsHandling(const std::vector<std::string> &argv);
        void handleFlag(const std::string &flag, const std::string &value);
    };
}  // namespace raytracer
