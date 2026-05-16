/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual header
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstddef>
#include <string>
#include <vector>

#include "Render.hpp"
#include "math/Color.hpp"
#include "visual/IVisual.hpp"

namespace raytracer::visual {

    class SFMLVisual : public IVisual {
      public:
        SFMLVisual();
        ~SFMLVisual() override;

        std::thread printProgress(int activeWorkers, Render &render) override;

        bool allowPreview() override;
        bool fullRender() override;
        bool installFile(Render &render) override;
        bool stopLoop() override;
        std::string selectScene(scenesMap &scene, Render &render) override;

      private:
        sf::RenderWindow _window;
        bool _fullRender = false;
        bool _stop = false;
        bool _save = false;
        bool _goBack = false;

        void displayText(float posX, float posY, const std::string &str);
        void updateWindowSize();
        void eventHandling(
            Render &render,
            const std::function<void(sf::Event &event)> &customCondition);

        // image rendering
        sf::Vector2f _windowSize;
        std::chrono::high_resolution_clock::time_point _start;
        std::vector<maths::Color> _cachedPreviewPixels;
        ImageSize _cacheImageSize;
        sf::Font _font;
        sf::Vector2u _imagePosition;

        float _marginTop = 0.20f;
        float _marginBottom = 0.20f;
        float _marginLeft = 0.2f;
        float _marginRight = 0.05f;

        void updateImageWindow(Render &render, ImageSize &imageSize);
        void dispayPixels(std::vector<maths::Color> &pixels, ImageSize &size);
        void eventImagePageHandling(sf::Event &event, Render &render);
        void computeImage(ImageSize &size, sf::Sprite &sprite);

        // selection scene
        std::vector<sf::FloatRect> _sceneButtonBounds;
        std::vector<std::string> _sceneNames;
        int _hoveredScene = -1;
        int _scrollOffset = 0;

        void drawSceneSelection(scenesMap &scenes);
        bool eventSelectionPageHandling(sf::Event &event, scenesMap &scenes,
                                        std::string &name);
    };
}  // namespace raytracer::visual
