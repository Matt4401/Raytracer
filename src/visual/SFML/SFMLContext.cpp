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
        _window.create(
            sf::VideoMode(1000, 1000), "Raytracer SFML",
            sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
        _window.setActive(false);
        updateWindowSize();
    }

    SFMLContext::~SFMLContext() {
        _window.close();
    }

    void SFMLContext::updateWindowSize() {
        sf::Vector2u s = _window.getSize();
        _windowSize =
            sf::Vector2f(static_cast<float>(s.x), static_cast<float>(s.y));
    }

    bool SFMLContext::loadFont(const std::string &path) {
        return _font.loadFromFile(path);
    }
}  // namespace raytracer::visual
