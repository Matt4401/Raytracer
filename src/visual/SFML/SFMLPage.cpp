/*
** EPITECH PROJECT, 2026
** SFMLPage
** File description:
** SFMLPage code
*/

#include "SFMLPage.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

namespace raytracer::visual {

    void SFMLPage::displayText(float posX, float posY, const std::string &str,
                               unsigned int size) {
        sf::Text text(str, this->_ctx.font());
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);

        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        text.setPosition(posX, posY);
        this->_ctx.window().draw(text);
    }

    void SFMLPage::checkEvents(
        Render &render, const std::function<void(sf::Event &)> &onEvent) {
        sf::Event event;
        auto &win = this->_ctx.window();

        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Resized) {
                win.setView(sf::View(
                    sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
            if (event.type == sf::Event::Closed) {
                render.stopRendering();
                this->_stop = true;
                win.close();
            }
            onEvent(event);
        }
    }

    void SFMLPage::runLoop(Render &render, const std::function<bool()> &stop,
                           const std::function<void()> &draw,
                           const std::function<void(sf::Event &)> &onEvent) {
        auto last = std::chrono::steady_clock::now();

        while (this->_ctx.window().isOpen() && !stop()) {
            auto now = std::chrono::steady_clock::now();
            double displayTimeOut =
                std::chrono::duration<double>(now - last).count();

            if (displayTimeOut >= 0.5) {
                this->_ctx.window().clear();
                draw();
                this->_ctx.window().display();
                last = now;
            }
            this->checkEvents(render, onEvent);
        }
    }

    void SFMLPage::drawButton(const sf::FloatRect &rect,
                              const std::string &label, unsigned int textSize) {
        sf::RectangleShape box(sf::Vector2f(rect.width, rect.height));
        box.setPosition(rect.left, rect.top);

        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_ctx.window());
        bool hovered = this->isMouseOver(rect, mousePos.x, mousePos.y);

        box.setFillColor(hovered ? sf::Color(80, 80, 120)
                                 : sf::Color(50, 50, 80));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        this->_ctx.window().draw(box);

        this->displayText(rect.left + rect.width / 2.f,
                          rect.top + rect.height / 2.f, label, textSize);
    }

    bool SFMLPage::isMouseOver(const sf::FloatRect &rect, int mx,
                               int my) const {
        return rect.contains(static_cast<float>(mx), static_cast<float>(my));
    }
}  // namespace raytracer::visual
