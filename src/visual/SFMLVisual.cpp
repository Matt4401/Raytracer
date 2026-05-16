/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual code
*/

#include "visual/SFMLVisual.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "Render.hpp"
#include "math/Color.hpp"

namespace raytracer::visual {

    SFMLVisual::SFMLVisual() {
        this->_window.create(
            sf::VideoMode(1000, 1000), "Raytracer SFML",
            sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

        this->_window.setActive(false);
    }

    SFMLVisual::~SFMLVisual() {
        this->_window.close();
    }

    bool SFMLVisual::allowPreview() {
        return true;
    }

    bool SFMLVisual::fullRender() {
        return this->_fullRender;
    }

    bool SFMLVisual::stopLoop() {
        return this->_stop || this->_save;
    }

    std::string SFMLVisual::selectScene(scenesMap &scenes, Render &render) {
        bool selected = false;
        std::string name;

        this->_goBack = false;
        this->_fullRender = false;
        this->_save = false;
        this->_cachedPreviewPixels.clear();
        this->_window.setActive(true);
        this->_font.loadFromFile("assets/arial.ttf");

        while (!selected && this->_window.isOpen() && !this->_stop) {
            this->_window.clear();
            this->updateWindowSize();
            this->drawSceneSelection(scenes);
            this->_window.display();

            this->eventHandling(
                render, [this, &selected, &name, &scenes](sf::Event &event) {
                    if (this->eventSelectionPageHandling(event, scenes, name))
                        selected = true;
                });
        }
        this->_window.setActive(false);
        return name;
    }

    void SFMLVisual::drawSceneSelection(scenesMap &scenes) {
        this->displayText(this->_windowSize.x * 0.5f,
                          this->_windowSize.y * 0.08f, "Select a Scene");

        this->_sceneButtonBounds.clear();
        this->_sceneNames.clear();

        const float buttonWidth = this->_windowSize.x * 0.6f;
        const float buttonHeight = 50.f;
        const float spacing = 10.f;
        const float startX = (this->_windowSize.x - buttonWidth) / 2.f;
        const float startY = this->_windowSize.y * 0.18f;

        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_window);
        sf::Vector2f mouseF(static_cast<float>(mousePos.x),
                            static_cast<float>(mousePos.y));

        int index = 0;
        for (const auto &pair : scenes) {
            const std::string &sceneName = pair.first;
            float y = startY + index * (buttonHeight + spacing);

            if (y + buttonHeight > this->_windowSize.y * 0.95f) {
                index++;
                continue;
            }

            sf::FloatRect bounds(startX, y, buttonWidth, buttonHeight);
            bool hovered = bounds.contains(mouseF);

            sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
            button.setPosition(startX, y);
            button.setFillColor(hovered ? sf::Color(80, 120, 200)
                                        : sf::Color(60, 60, 80));
            button.setOutlineThickness(2.f);
            button.setOutlineColor(sf::Color(150, 150, 180));
            this->_window.draw(button);

            sf::Text text(sceneName, this->_font);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            sf::FloatRect tb = text.getLocalBounds();
            text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            text.setPosition(startX + buttonWidth / 2.f,
                             y + buttonHeight / 2.f);
            this->_window.draw(text);

            this->_sceneButtonBounds.push_back(bounds);
            this->_sceneNames.push_back(sceneName);
            index++;
        }
    }

    bool SFMLVisual::eventSelectionPageHandling(sf::Event &event,
                                                scenesMap &scenes,
                                                std::string &name) {
        (void)scenes;
        if (event.type != sf::Event::MouseButtonPressed)
            return false;
        if (event.mouseButton.button != sf::Mouse::Left)
            return false;

        sf::Vector2f click(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));

        for (size_t i = 0; i < this->_sceneButtonBounds.size(); i++) {
            if (this->_sceneButtonBounds[i].contains(click)) {
                name = this->_sceneNames[i];
                return true;
            }
        }
        return false;
    }

    bool SFMLVisual::wantSave(Render &render) {
        this->_window.setActive(true);
        std::chrono::time_point last = std::chrono::steady_clock::now();

        while (this->_window.isOpen() && !this->_save && !this->_goBack) {
            std::chrono::time_point now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - last).count();
            ImageSize imageSize = render.imageSize();

            if (elapsed >= 1.0) {
                this->_window.clear();
                this->updateImageWindow(render, imageSize);

                this->displayText(this->_windowSize.x * 0.5f,
                                  this->_windowSize.y * 0.9f,
                                  "Click on \"S\" to save this file");
                this->_window.display();
                last = now;
            }
            this->eventHandling(render, [this, &render](sf::Event &event) {
                this->eventImagePageHandling(event, render);
            });
        }
        this->_window.setActive(false);
        return this->_save;
    }

    std::thread SFMLVisual::printProgress(int activeWorkers, Render &render) {
        return std::thread([this, &render, activeWorkers]() {
            (void)activeWorkers;

            std::function<void()> callEvent = [this, &render]() {
                this->eventHandling(render, [this, &render](sf::Event &event) {
                    this->eventImagePageHandling(event, render);
                });
            };

            this->_window.setActive(true);
            ImageSize imageSize = render.imageSize();
            this->_font.loadFromFile("assets/arial.ttf");

            std::chrono::time_point last = std::chrono::steady_clock::now();

            while (!render.renderingIsFinished() && this->_window.isOpen() &&
                   !this->_goBack) {
                std::chrono::time_point now = std::chrono::steady_clock::now();
                double elapsed =
                    std::chrono::duration<double>(now - last).count();

                if (elapsed >= 1.0) {
                    this->_window.clear();
                    this->updateImageWindow(render, imageSize);
                    this->_window.display();
                    last = now;
                }
                callEvent();
            }
            this->_window.clear();
            this->updateImageWindow(render, imageSize);
            this->_window.display();
            while (this->_window.isOpen() && !this->fullRender() &&
                   !this->_goBack) {
                callEvent();
            }
            if (this->_fullRender && this->_cachedPreviewPixels.empty()) {
                this->_cachedPreviewPixels = render.pixels();
                this->_cacheImageSize = imageSize;
            }
            this->_window.setActive(false);
        });
    }

    void SFMLVisual::updateWindowSize() {
        sf::Vector2u windowSize = this->_window.getSize();
        this->_windowSize = sf::Vector2f(static_cast<float>(windowSize.x),
                                         static_cast<float>(windowSize.y));
    }

    void SFMLVisual::updateImageWindow(Render &render, ImageSize &imageSize) {
        this->updateWindowSize();

        if (!this->_cachedPreviewPixels.empty()) {
            this->dispayPixels(this->_cachedPreviewPixels,
                               this->_cacheImageSize);
        }
        this->dispayPixels(render.pixels(), imageSize);
        this->displayText(
            this->_windowSize.x * 0.5f, this->_windowSize.y * 0.1f,
            this->_fullRender ? "Full Render Mode" : "preview Mode");
    }

    void SFMLVisual::displayText(float posX, float posY,
                                 const std::string &str) {
        sf::Text mode(str, this->_font);

        mode.setPosition(posX, posY);
        mode.setFillColor(sf::Color::White);
        mode.setCharacterSize(40);

        sf::FloatRect textBounds = mode.getLocalBounds();
        mode.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);
        this->_window.draw(mode);
    }

    void SFMLVisual::dispayPixels(std::vector<maths::Color> &pixels,
                                  ImageSize &size) {
        sf::Image image;

        image.create(size.width, size.heigth, sf::Color::Transparent);
        for (int y = 0; y < size.heigth; y++) {
            for (int x = 0; x < size.width; x++) {
                maths::Color &color = pixels.at(y * size.width + x);

                if ((color.r == 0 && color.g == 0 && color.b == 0))
                    continue;
                image.setPixel(x, y, sf::Color(color.r, color.g, color.b));
            }
        }
        sf::Texture texture;
        sf::Sprite sprite;

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        this->computeImage(size, sprite);

        this->_window.draw(sprite);
    }

    void SFMLVisual::computeImage(ImageSize &size, sf::Sprite &sprite) {
        sf::Vector2f &windowSize = this->_windowSize;

        float availableWidth =
            std::max(0.f, windowSize.x * (1.0f - _marginLeft - _marginRight));
        float availableHeight =
            std::max(0.f, windowSize.y * (1.0f - _marginTop - _marginBottom));

        float scaleX = availableWidth / static_cast<float>(size.width);
        float scaleY = availableHeight / static_cast<float>(size.heigth);
        float scale = std::max(0.f, std::min(scaleX, scaleY));

        if (scale <= 0.f)
            return;
        float drawnWidth = size.width * scale;
        float drawnHeight = size.heigth * scale;
        float posX =
            windowSize.x * _marginLeft + (availableWidth - drawnWidth) / 2.f;
        float posY =
            windowSize.y * _marginTop + (availableHeight - drawnHeight) / 2.f;

        sprite.setScale(scale, scale);
        sprite.setPosition(posX, posY);
    }

    void SFMLVisual::eventImagePageHandling(sf::Event &event, Render &render) {
        if (event.type != sf::Event::KeyPressed)
            return;
        switch (event.key.code) {
            case sf::Keyboard::Backspace:
                this->_goBack = true;
                render.stopRendering();
                break;
            case sf::Keyboard::F:
                this->_fullRender = true;
                render.stopRendering();
                break;
            case sf::Keyboard::S:
                if (render.renderingIsFinished() && this->_fullRender)
                    this->_save = true;
                break;
            default:
                break;
        }
    }

    void SFMLVisual::eventHandling(
        Render &render,
        const std::function<void(sf::Event &event)> &customCondition) {
        sf::Event event;

        while (this->_window.pollEvent(event)) {
            if (event.type == sf::Event::Resized) {
                this->_window.setView(sf::View(
                    sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
            if (event.type == sf::Event::Closed) {
                render.stopRendering();
                this->_stop = true;
                this->_window.close();
            }
            customCondition(event);
        }
    }

}  // namespace raytracer::visual
