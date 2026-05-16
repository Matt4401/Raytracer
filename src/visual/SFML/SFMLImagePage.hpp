/*
** EPITECH PROJECT, 2026
** SFMLImagePage
** File description:
** SFMLImagePage header
*/

#pragma once

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

        void reset();

        std::thread printProgress(Render &render);

        bool installFile(Render &render);

        bool fullRender() const {
            return _fullRender;
        }
        bool save() const {
            return _save;
        }
        bool back() const {
            return _goBack;
        }

      private:
        bool _fullRender = false;
        bool _save = false;
        bool _goBack = false;

        float _marginTop = 0.15f;
        float _marginBottom = 0.10f;
        float _marginLeft = 0.10f;
        float _marginRight = 0.10f;

        std::vector<maths::Color> _cachedPreviewPixels;
        ImageSize _cacheImageSize;

        void drawFrame(Render &render, ImageSize &imageSize);
        void drawPixels(std::vector<maths::Color> &pixels, ImageSize &size);
        void layoutSprite(ImageSize &size, sf::Sprite &sprite);
        void handleEvent(sf::Event &event, Render &render);
    };
}  // namespace raytracer::visual
