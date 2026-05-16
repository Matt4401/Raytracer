/*
** EPITECH PROJECT, 2026
** SFMLContext
** File description:
** SFMLContext code
*/

#include "SFMLContext.hpp"

#include <SFML/Window/VideoMode.hpp>

namespace raytracer::visual {

    SFMLContext::SFMLContext() {
        this->_window.create(
            sf::VideoMode(1000, 1000), "Raytracer SFML",
            sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
        this->_window.setActive(false);
        this->updateWindowSize();
    }

    SFMLContext::~SFMLContext() {
        this->_window.close();
    }

    void SFMLContext::updateWindowSize() {
        sf::Vector2u s = this->_window.getSize();
        this->_windowSize =
            sf::Vector2f(static_cast<float>(s.x), static_cast<float>(s.y));
    }

    bool SFMLContext::loadFont(const std::string &path) {
        return this->_font.loadFromFile(path);
    }
}  // namespace raytracer::visual
