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

        /**
         * @brief Get a reference to the SFML render window.
         * @return A reference to the sf::RenderWindow object.
         */
        sf::RenderWindow &window() {
            return _window;
        }
        /**
         * @brief Get the loaded font used for text rendering.
         * @return A const reference to the sf::Font object.
         */
        const sf::Font &font() const {
            return _font;
        }
        /**
         * @brief Get the current window size.
         * @return A const reference to the window size as sf::Vector2f.
         */
        const sf::Vector2f &windowSize() const {
            return _windowSize;
        }

        /**
         * @brief Update the cached window size based on current window
         * dimensions. This should be called whenever the window is resized.
         */
        void updateWindowSize();
        /**
         * @brief Load a font from a file path.
         * @param path The file path to the font file.
         * @return true if the font was successfully loaded, false otherwise.
         */
        bool loadFont(const std::string &path);

      private:
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Vector2f _windowSize;
    };
}  // namespace raytracer::visual
