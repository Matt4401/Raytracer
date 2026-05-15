/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual header
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "Render.hpp"
#include "math/Color.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class SFMLVisual : public IVisual {
      public:
        SFMLVisual();
        ~SFMLVisual() override;

        std::thread printProgress(int activeWorkers, Render &render) override;

        bool allowPreview() override;
        bool fullRender() override;

      private:
        sf::RenderWindow _window;
        std::chrono::high_resolution_clock::time_point _start;
        sf::Image _image;
        sf::Font _font;
        sf::Vector2u _imagePosition;
        bool _fullRender = false;

        float _marginTop = 0.05f;
        float _marginBottom = 0.20f;
        float _marginLeft = 0.2f;
        float _marginRight = 0.05f;

        void updateWindow(Render &render, ImageSize &imageSize);

        void dispayPixels(std::vector<maths::Color> &pixels, ImageSize &size,
                          sf::Vector2f &windowSize);
        void displayCurrentMode(sf::Vector2f &windowSize);
        void computeImage(ImageSize &size, sf::Vector2f &windowSize,
                          sf::Sprite &sprite);

        void eventHandling(Render &render);
        void keyPressHandling(sf::Event &event, Render &render);
    };
}  // namespace raytracer::visual
