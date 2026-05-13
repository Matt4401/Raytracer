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
    };

}  // namespace raytracer::visual
