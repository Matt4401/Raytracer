// SFMLVisual.hpp
#pragma once

#include "visual/IVisual.hpp"
#include "visual/SFML/SFMLContext.hpp"
#include "visual/SFML/SFMLImagePage.hpp"
#include "visual/SFML/SFMLSelectionPage.hpp"

namespace raytracer::visual {

    class SFMLVisual : public IVisual {
      public:
        SFMLVisual() : _selection(_ctx), _image(_ctx) {
            _ctx.loadFont("assets/arial.ttf");
        }
        ~SFMLVisual() override = default;

        /**
         * @brief Check if the SFML interface supports live preview during
         * rendering.
         * @return true (SFML always supports preview).
         */
        bool allowPreview() override {
            return true;
        }
        /**
         * @brief Request a full quality render with complete sample count.
         * @return true if full render is requested, false to use preview
         * sampling.
         */
        bool fullRender() override {
            return _image.fullRender();
        }
        /**
         * @brief Check if the SFML visual loop should stop.
         * Stops if user requests stop, selection page stops, or user requests
         * to save.
         * @return true if loop should stop, false to continue running.
         */
        bool stopLoop() override {
            return _image.stopRequested() || _selection.stopRequested() ||
                   _image.save();
        }

        /**
         * @brief Run the SFML scene selection interface and reset image page.
         * @param scenes Map of available scenes indexed by their file paths.
         * @param render The render context for the selected scene.
         * @return The name/identifier of the selected scene.
         */
        std::string selectScene(scenesMap &scenes, Render &render) override {
            _image.reset();
            return _selection.run(scenes, render);
        }

        /**
         * @brief Display rendering progress in the SFML window.
         * @param activeWorkers Unused parameter (for interface compatibility).
         * @param render The render context containing rendering state.
         * @return A thread handle for the progress display task.
         */
        std::thread printProgress(int, Render &render) override {
            return _image.printProgress(render);
        }

        /**
         * @brief Ask the user if they want to save the rendered result via SFML
         * interface.
         * @param render The render context containing the rendered data.
         * @return true if the user wants to save, false otherwise.
         */
        bool wantSave(Render &render) override {
            return _image.wantSave(render);
        }

      private:
        SFMLContext _ctx;
        SFMLSelectionPage _selection;
        SFMLImagePage _image;
    };
}  // namespace raytracer::visual
