/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual implementation
*/

#include "SFMLVisual.hpp"

namespace raytracer::visual {

    SFMLVisual::SFMLVisual() : _selection(_ctx), _image(_ctx) {
        _ctx.loadFont("assets/arial.ttf");
    }

    bool SFMLVisual::allowPreview() {
        return true;
    }

    bool SFMLVisual::fullRender() {
        return _image.fullRender();
    }

    bool SFMLVisual::stopLoop() {
        return _image.stopRequested() || _selection.stopRequested() ||
               _image.save();
    }

    int SFMLVisual::selectScene(
        std::vector<object::scene::SceneInstance> &scenes, Render &render) {
        _image.reset();
        return _selection.run(scenes, render);
    }

    std::thread SFMLVisual::printProgress(int, Render &render) {
        return _image.printProgress(render);
    }

    bool SFMLVisual::wantSave(Render &render) {
        return _image.wantSave(render);
    }

    bool SFMLVisual::isBackRequested() {
        return _image.back();
    }

}  // namespace raytracer::visual
