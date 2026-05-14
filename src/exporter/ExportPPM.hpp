/*
** EPITECH PROJECT, 2026
** ExportPPM
** File description:
** ExportPPM header
*/

#pragma once

#include "IExport.hpp"
namespace raytracer::exporter {

    class ExportPPM : public IExport {
      public:
        ExportPPM() = default;
        ~ExportPPM() override = default;

        void writeFile(const raytracer::object::scene::IScene &scene,
                       const std::vector<maths::Color> &pixels) const override;

      private:
        static void writeColor(const maths::Color color);
    };

}  // namespace raytracer::exporter
