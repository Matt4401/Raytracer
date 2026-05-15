/*
** EPITECH PROJECT, 2026
** IVisual
** File description:
** IVisual header
*/

#pragma once

#include <thread>

#include "Render.hpp"
namespace raytracer::visual {
    class IVisual {
      public:
        virtual ~IVisual() = default;

        virtual std::thread printProgress(int activeWorkers,
                                          Render &render) = 0;

        virtual bool allowPreview() = 0;
        virtual bool fullRender() = 0;
    };

}  // namespace raytracer::visual
