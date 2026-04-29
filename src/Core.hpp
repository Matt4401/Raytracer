/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#ifndef CORE_HPP_
#define CORE_HPP_

#include <filesystem>

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
        void init(const std::filesystem::path &file);

        /**
         * @brief Run the main loop
         *
         * TODO: Implement the main core program
         */
        void run();

      private:
        PluginManager _plugManager;
        ObjectFactory _objFactory;

        static constexpr std::string_view PLUGINS_FOLDER_PATH = "./plugins/";
    };
}  // namespace raytracer

#endif /* !CORE_HPP_ */
