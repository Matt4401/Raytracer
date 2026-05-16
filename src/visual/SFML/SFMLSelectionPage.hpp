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

        /**
         * @brief Run the scene selection interface.
         * Displays available scenes and allows the user to select one.
         * @param scenes Map of available scenes indexed by their file paths.
         * @param render The render context to pass to the selected scene.
         * @return The name/identifier of the selected scene.
         */
        std::string run(IVisual::scenesMap &scenes, Render &render);

      private:
        std::vector<sf::FloatRect> _buttonBounds;
        std::vector<std::string> _sceneNames;
        float _buttonWidth = 50.f;
        float _buttonHeight = 50.f;

        static constexpr float SPACING = 20.f;

        void draw(IVisual::scenesMap &scenes);

        bool handleEvent(sf::Event &event, std::string &outName);
    };
}  // namespace raytracer::visual
