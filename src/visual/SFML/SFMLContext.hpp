/*
** EPITECH PROJECT, 2026
** SFMLContext
** File description:
** SFMLContext header
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

namespace raytracer::visual {

    class SFMLContext {
      public:
        SFMLContext();
        ~SFMLContext();

        sf::RenderWindow &window() {
            return _window;
        }
        const sf::Font &font() const {
            return _font;
        }
        const sf::Vector2f &windowSize() const {
            return _windowSize;
        }

        void updateWindowSize();
        bool loadFont(const std::string &path);

      private:
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Vector2f _windowSize;
    };
}  // namespace raytracer::visual
