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
#include <string>
#include <string_view>
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
        bool installFile(Render &render) override;

      private:
        sf::RenderWindow _window;
        sf::Vector2f _windowSize;
        std::chrono::high_resolution_clock::time_point _start;
        sf::Image _image;
        sf::Font _font;
        sf::Vector2u _imagePosition;

        bool _fullRender = false;
        bool _save = false;

        float _marginTop = 0.20f;
        float _marginBottom = 0.20f;
        float _marginLeft = 0.2f;
        float _marginRight = 0.05f;

        void updateWindow(Render &render, ImageSize &imageSize);
        void updateWindowSize();

        void dispayPixels(std::vector<maths::Color> &pixels, ImageSize &size);
        void displayText(float posX, float posY, const std::string &str);
        void computeImage(ImageSize &size, sf::Sprite &sprite);

        void eventHandling(Render &render);
        void keyPressHandling(sf::Event &event, Render &render);
    };
}  // namespace raytracer::visual
