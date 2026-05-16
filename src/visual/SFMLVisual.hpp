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

        bool allowPreview() override {
            return true;
        }
        bool fullRender() override {
            return _image.fullRender();
        }
        bool stopLoop() override {
            return _image.stopRequested() || _selection.stopRequested() ||
                   _image.save();
        }

        std::string selectScene(scenesMap &scenes, Render &render) override {
            _image.reset();
            return _selection.run(scenes, render);
        }

        std::thread printProgress(int, Render &render) override {
            return _image.printProgress(render);
        }

        bool installFile(Render &render) override {
            return _image.installFile(render);
        }

      private:
        SFMLContext _ctx;
        SFMLSelectionPage _selection;
        SFMLImagePage _image;
    };
}  // namespace raytracer::visual
