/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual header
*/

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Render.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class SFMLVisual : public IVisual {
      public:
        SFMLVisual();
        ~SFMLVisual() override = default;

        std::thread printProgress(int activeWorkers, Render &render) override;

      private:
        sf::RenderWindow _window;
        std::chrono::high_resolution_clock::time_point _start;
        sf::Image _image;
        sf::Texture _texture;

        void dispayPixels(std::vector<maths::Color> &pixels, ImageSize size);
        void eventHandling(Render &render);
    };
}  // namespace raytracer::visual
