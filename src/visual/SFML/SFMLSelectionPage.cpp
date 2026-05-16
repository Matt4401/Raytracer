/*
** EPITECH PROJECT, 2026
** SFMLSelectionPage
** File description:
** SFMLSelectionPage code
*/

#include "SFMLSelectionPage.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "SFMLSelectionPage.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    std::string SFMLSelectionPage::run(IVisual::scenesMap &scenes,
                                       Render &render) {
        std::string name;
        bool selected = false;

        this->_ctx.window().setActive(true);

        while (!selected && _ctx.window().isOpen() && !_stop) {
            this->_ctx.window().clear();
            this->_ctx.updateWindowSize();
            draw(scenes);
            this->_ctx.window().display();

            checkEvents(render, [&](sf::Event &e) {
                if (handleEvent(e, name))
                    selected = true;
            });
        }
        this->_ctx.window().setActive(false);
        return name;
    }

    void SFMLSelectionPage::draw(IVisual::scenesMap &scenes) {
        const sf::Vector2f &ws = _ctx.windowSize();
        this->_buttonWidth = ws.x * 0.6f;
        const float startY = ws.y * 0.18f;
        const float startX = (ws.x - this->_buttonWidth) / 2.f;
        int index = 0;

        this->_buttonBounds.clear();
        this->_sceneNames.clear();

        displayText(ws.x * 0.5f, ws.y * 0.08f, "Select a Scene");

        for (const auto &pair : scenes) {
            float y = startY + index * (this->_buttonHeight + SPACING);
            if (y + this->_buttonHeight > ws.y * 0.95f) {
                index++;
                continue;
            }
            sf::FloatRect bounds(startX, y, this->_buttonWidth,
                                 this->_buttonHeight);
            this->drawButton(bounds, pair.first);
            this->_buttonBounds.push_back(bounds);
            this->_sceneNames.push_back(pair.first);
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
