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

namespace raytracer::visual {

    class SFMLSelectionPage : public SFMLPage {
      public:
        using SFMLPage::SFMLPage;

        /**
         * @brief Run the scene selection interface.
         * Displays available scenes and allows the user to select one.
         * @param scenes Vector of available scenes.
         * @param render The render context to pass to the selected scene.
         * @return The index of the selected scene, or a negative value to
         * indicate exit.
         */
        int run(std::vector<object::scene::SceneInstance> &scenes,
                Render &render);

      private:
        std::vector<sf::FloatRect> _buttonBounds;
        std::vector<std::string> _sceneNames;
        float _buttonWidth = 50.f;
        float _buttonHeight = 50.f;

        static constexpr float SPACING = 20.f;

        void draw(std::vector<object::scene::SceneInstance> &scenes);

        bool handleEvent(sf::Event &event, int &outIndex);
    };
}  // namespace raytracer::visual
