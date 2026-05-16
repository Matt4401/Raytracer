/*
** EPITECH PROJECT, 2026
** SFMLImagePage
** File description:
** SFMLImagePage header
*/

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <thread>
#include <vector>

#include "Render.hpp"
#include "SFMLPage.hpp"
#include "math/Color.hpp"

namespace raytracer::visual {

    class SFMLImagePage : public SFMLPage {
      public:
        using SFMLPage::SFMLPage;

        /**
         * @brief Reset the image page to initial state.
         * Clears cached pixels and resets UI state flags.
         */
        void reset();

        /**
         * @brief Display rendering progress in the image page.
         * @param render The render context containing rendering state.
         * @return A thread handle for the progress display task.
         */
        std::thread printProgress(Render &render);

        /**
         * @brief Handle file installation/export from the image page.
         * @param render The render context containing the rendered image data.
         * @return true if file installation was successful, false otherwise.
         */
        bool wantSave(Render &render);

        /**
         * @brief Check if a full render operation is in progress.
         * @return true if full render is active, false otherwise.
         */
        bool fullRender() const {
            return _fullRender;
        }
        /**
         * @brief Check if the user has requested to save the rendered image.
         * @return true if save is requested, false otherwise.
         */
        bool save() const {
            return _save;
        }
        /**
         * @brief Check if the user has requested to go back to the previous
         * page.
         * @return true if back navigation is requested, false otherwise.
         */
        bool back() const {
            return _goBack;
        }

      private:
        bool _fullRender = false;
        bool _save = false;
        bool _goBack = false;

        float _marginTop = 0.0f;
        float _marginBottom = 0.0f;
        float _marginLeft = 0.10f;
        float _marginRight = 0.10f;

        std::vector<maths::Color> _cachedPreviewPixels;
        ImageSize _cacheImageSize;

        // Button rectangles (in window coordinates), updated each frame
        sf::FloatRect _fullRenderButton;
        sf::FloatRect _saveButton;
        bool _showSaveButton = false;

        bool isActive() const {
            return _ctx.window().isOpen() && !_goBack;
        }

        void drawFrame(Render &render, ImageSize &imageSize);
        void drawPixels(std::vector<maths::Color> &pixels, ImageSize &size);
        void layoutSprite(ImageSize &size, sf::Sprite &sprite);
        void handleEvent(sf::Event &event, Render &render);
        void drawButton(const sf::FloatRect &rect, const std::string &label);
        bool isMouseOver(const sf::FloatRect &rect, int mx, int my) const;
    };
}  // namespace raytracer::visual
