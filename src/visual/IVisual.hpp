/*
** EPITECH PROJECT, 2026
** IVisual
** File description:
** IVisual header
*/

#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <thread>

#include "Render.hpp"

namespace raytracer::visual {
    class IVisual {
      public:
        virtual ~IVisual() = default;

        /**
         * @brief Display rendering progress with active worker count.
         * @param activeWorkers The number of currently active rendering
         * workers.
         * @param render The render context containing rendering information.
         * @return A thread handle for the progress printing task.
         */
        virtual std::thread printProgress(int activeWorkers,
                                          Render &render) = 0;

        using scenesMap =
            const std::map<std::filesystem::path,
                           std::shared_ptr<object::scene::IScene>>;
        /**
         * @brief Check if the visual interface supports live preview during
         * rendering.
         * @return true if preview is allowed, false otherwise.
         */
        virtual bool allowPreview() = 0;
        /**
         * @brief Determine if a full render operation should be performed.
         * @return true if a full render is requested, false otherwise.
         */
        virtual bool fullRender() = 0;

        /**
         * @brief Ask the user if they want to save the rendered result.
         * @param render The render context containing the rendered data.
         * @return true if the user wants to save, false otherwise.
         */
        virtual bool wantSave(Render &render) = 0;
        /**
         * @brief Check if the visual loop should stop.
         * @return true if the loop should stop, false to continue running.
         */
        virtual bool stopLoop() = 0;
        /**
         * @brief Allow the user to select a scene from available scenes.
         * @param scene Map of available scenes indexed by their file paths.
         * @param render The render context for the selected scene.
         * @return The name/identifier of the selected scene.
         */
        virtual std::string selectScene(scenesMap &scene, Render &render) = 0;
    };

}  // namespace raytracer::visual
