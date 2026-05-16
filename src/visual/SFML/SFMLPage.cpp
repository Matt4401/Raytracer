/*
** EPITECH PROJECT, 2026
** SFMLPage
** File description:
** SFMLPage code
*/

#include "SFMLPage.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

namespace raytracer::visual {

    void SFMLPage::displayText(float posX, float posY, const std::string &str,
                               unsigned int size) {
        sf::Text text(str, _ctx.font());
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);

        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        text.setPosition(posX, posY);
        _ctx.window().draw(text);
    }

    void SFMLPage::checkEvents(Render &render,
                              const std::function<void(sf::Event &)> &onEvent) {
        sf::Event event;
        auto &win = _ctx.window();

        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Resized) {
                win.setView(sf::View(
                    sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
            if (event.type == sf::Event::Closed) {
                render.stopRendering();
                _stop = true;
                win.close();
            }
            onEvent(event);
        }
    }
}  // namespace raytracer::visual
