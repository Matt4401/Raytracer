/*
** EPITECH PROJECT, 2026
** IExport
** File description:
** IExport header
*/

#pragma once

#include "object/IScene.hpp"
namespace raytracer::exporter {

    class IExport {
      public:
        virtual ~IExport() = default;

        /// @brief write the rendered image in std::cout.
        virtual void writeFile(
            const raytracer::object::scene::IScene &scene,
            const std::vector<maths::Color> &pixels) const = 0;
        // Methods
    };

}  // namespace raytracer::exporter
