/*
** EPITECH PROJECT, 2026
** CliVisual
** File description:
** CliVisual header
*/

#pragma once

#include "object/IScene.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class CliVisual : public IVisual {
      public:
        CliVisual() = default;
        ~CliVisual() override = default;

        /**
         * @brief Display rendering progress in the command-line interface.
         * @param activeWorkers The number of currently active rendering
         * workers.
         * @param render The render context containing rendering information.
         * @return A thread handle for the progress printing task.
         */
        std::thread printProgress(int activeWorkers, Render &render) override;

        /**
         * @brief Check if the CLI interface supports live preview during
         * rendering.
         * @return true if preview is allowed in CLI mode, false otherwise.
         */
        bool allowPreview() override;
        /**
         * @brief Determine if a full render operation should be performed.
         * @return true if a full render is requested, false otherwise.
         */
        bool fullRender() override;
        /**
         * @brief Ask the user if they want to save the rendered result in CLI
         * mode.
         * @param render The render context containing the rendered data.
         * @return true if the user wants to save, false otherwise.
         */
        bool wantSave(Render &render) override;
        /**
         * @brief Check if the CLI loop should stop.
         * @return true if the loop should stop, false to continue running.
         */
        bool stopLoop() override;
        /**
         * @brief Allow the user to select a scene from available scenes via
         * CLI.
         * @param scene Vector of available scenes.
         * @param render The render context for the selected scene.
         * @return The index of the selected scene, or a negative value to
         * indicate exit.
         */
        int selectScene(std::vector<object::scene::SceneInstance> &scenes,
                        Render &render) override;

        bool isBackRequested() override;

      private:
        bool _stop = false;
    };

}  // namespace raytracer::visual
