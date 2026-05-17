/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual header
*/

#pragma once

#include <vector>

#include "visual/IVisual.hpp"
#include "visual/SFML/SFMLContext.hpp"
#include "visual/SFML/SFMLImagePage.hpp"
#include "visual/SFML/SFMLSelectionPage.hpp"

namespace raytracer::visual {

    class SFMLVisual : public IVisual {
      public:
        SFMLVisual();
        ~SFMLVisual() override = default;

        /**
         * @brief Check if the SFML interface supports live preview during
         * rendering.
         * @return true (SFML always supports preview).
         */
        bool allowPreview() override;

        /**
         * @brief Request a full quality render with complete sample count.
         * @return true if full render is requested, false to use preview
         * sampling.
         */
        bool fullRender() override;

        /**
         * @brief Check if the SFML visual loop should stop.
         * Stops if user requests stop, selection page stops, or user requests
         * to save.
         * @return true if loop should stop, false to continue running.
         */
        bool stopLoop() override;

        /**
         * @brief Run the SFML scene selection interface and reset image page.
         * @param scenes Vector of available scenes.
         * @param render The render context for the selected scene.
         * @return The index of the selected scene, or a negative value to
         * indicate exit.
         */
        int selectScene(std::vector<object::scene::SceneInstance> &scenes,
                        Render &render) override;

        /**
         * @brief Display rendering progress in the SFML window.
         * @param activeWorkers Unused parameter (for interface compatibility).
         * @param render The render context containing rendering state.
         * @return A thread handle for the progress display task.
         */
        std::thread printProgress(int activeWorkers, Render &render) override;

        /**
         * @brief Ask the user if they want to save the rendered result via SFML
         * interface.
         * @param render The render context containing the rendered data.
         * @return true if the user wants to save, false otherwise.
         */
        bool wantSave(Render &render) override;

        /**
         * @brief Check if the user requested to go back to scene selection.
         * @return true if back navigation was requested, false otherwise.
         */
        bool isBackRequested() override;

      private:
        SFMLContext _ctx;
        SFMLSelectionPage _selection;
        SFMLImagePage _image;
    };

}  // namespace raytracer::visual
