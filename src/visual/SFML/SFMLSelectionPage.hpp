/*
** EPITECH PROJECT, 2026
** SFMLSelectionPage
** File description:
** SFMLSelectionPage header
*/

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>

#include "Render.hpp"
#include "SFMLPage.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class SFMLSelectionPage : public SFMLPage {
      public:
        using SFMLPage::SFMLPage;

        std::string run(IVisual::scenesMap &scenes, Render &render);

      private:
        std::vector<sf::FloatRect> _buttonBounds;
        std::vector<std::string> _sceneNames;

        void draw(IVisual::scenesMap &scenes);

        bool handleEvent(sf::Event &event, std::string &outName);
    };
}  // namespace raytracer::visual
