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

        virtual std::thread printProgress(int activeWorkers,
                                          Render &render) = 0;

        using scenesMap =
            const std::map<std::filesystem::path,
                           std::shared_ptr<object::scene::IScene>>;
        virtual bool allowPreview() = 0;
        virtual bool fullRender() = 0;
        virtual bool installFile(Render &render) = 0;
        virtual bool stopLoop() = 0;
        virtual std::string selectScene(scenesMap &scene, Render &render) = 0;
    };

}  // namespace raytracer::visual
