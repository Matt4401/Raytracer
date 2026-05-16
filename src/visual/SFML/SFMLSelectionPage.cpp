/*
** EPITECH PROJECT, 2026
** SFMLSelectionPage
** File description:
** SFMLSelectionPage code
*/

#include "SFMLSelectionPage.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>

#include "visual/IVisual.hpp"

namespace raytracer::visual {

    std::string SFMLSelectionPage::run(IVisual::scenesMap &scenes,
                                       Render &render) {
        std::string name;
        bool selected = false;

        _ctx.window().setActive(true);

        while (!selected && _ctx.window().isOpen() && !_stop) {
            _ctx.window().clear();
            _ctx.updateWindowSize();
            draw(scenes);
            _ctx.window().display();

            checkEvents(render, [&](sf::Event &e) {
                if (handleEvent(e, name))
                    selected = true;
            });
        }
        _ctx.window().setActive(false);
        return name;
    }

    void SFMLSelectionPage::draw(IVisual::scenesMap &scenes) {
        const auto &ws = _ctx.windowSize();
        auto &win = _ctx.window();

        displayText(ws.x * 0.5f, ws.y * 0.08f, "Select a Scene");

        _buttonBounds.clear();
        _sceneNames.clear();

        const float bw = ws.x * 0.6f;
        const float bh = 50.f;
        const float spacing = 10.f;
        const float startX = (ws.x - bw) / 2.f;
        const float startY = ws.y * 0.18f;

        sf::Vector2i mp = sf::Mouse::getPosition(win);
        sf::Vector2f mouseF(static_cast<float>(mp.x), static_cast<float>(mp.y));

        int index = 0;
        for (const auto &pair : scenes) {
            const std::string &sceneName = pair.first;
            float y = startY + index * (bh + spacing);
            if (y + bh > ws.y * 0.95f) {
                index++;
                continue;
            }

            sf::FloatRect bounds(startX, y, bw, bh);
            bool hovered = bounds.contains(mouseF);

            sf::RectangleShape btn(sf::Vector2f(bw, bh));
            btn.setPosition(startX, y);
            btn.setFillColor(hovered ? sf::Color(80, 120, 200)
                                     : sf::Color(60, 60, 80));
            btn.setOutlineThickness(2.f);
            btn.setOutlineColor(sf::Color(150, 150, 180));
            win.draw(btn);

            sf::Text text(sceneName, _ctx.font());
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            sf::FloatRect tb = text.getLocalBounds();
            text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            text.setPosition(startX + bw / 2.f, y + bh / 2.f);
            win.draw(text);

            _buttonBounds.push_back(bounds);
            _sceneNames.push_back(sceneName);
            index++;
        }
    }

    bool SFMLSelectionPage::handleEvent(sf::Event &event,
                                        std::string &outName) {
        if (event.type != sf::Event::MouseButtonPressed)
            return false;
        if (event.mouseButton.button != sf::Mouse::Left)
            return false;

        sf::Vector2f click(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        for (size_t i = 0; i < _buttonBounds.size(); i++) {
            if (_buttonBounds[i].contains(click)) {
                outName = _sceneNames[i];
                return true;
            }
        }
        return false;
    }
}  // namespace raytracer::visual
