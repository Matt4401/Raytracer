/*
** EPITECH PROJECT, 2026
** CliVisual
** File description:
** CliVisual header
*/

#pragma once

#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class CliVisual : public IVisual {
      public:
        CliVisual() = default;
        ~CliVisual() override = default;

        std::thread printProgress(int activeWorkers, Render &render) override;

        bool allowPreview() override;
        bool fullRender() override;
        bool installFile(Render &render) override;
        bool stopLoop() override;
        std::string selectScene(IVisual::scenesMap &scene,
                                Render &render) override;

      private:
        bool _stop = false;
    };

}  // namespace raytracer::visual
