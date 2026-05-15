/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual code
*/

#include "visual/SFMLVisual.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <chrono>
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

    bool SFMLVisual::installFile(Render &render) {
        this->_window.setActive(true);
        std::chrono::time_point last = std::chrono::steady_clock::now();

        while (this->_window.isOpen() && !this->_save) {
            std::chrono::time_point now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - last).count();
            ImageSize imageSize = render.imageSize();

            if (elapsed >= 1.0) {
                this->_window.clear();
                this->updateWindow(render, imageSize);

                this->displayText(this->_windowSize.x * 0.5f,
                                  this->_windowSize.y * 0.9f,
                                  "Click on \"S\" to save this file");
                this->_window.display();
                last = now;
            }

            this->eventHandling(render);
        }

        this->_window.setActive(false);

        return this->_save;
    }

    std::thread SFMLVisual::printProgress(int activeWorkers, Render &render) {
        return std::thread([this, &render, activeWorkers]() {
            (void)activeWorkers;

            this->_window.setActive(true);
            ImageSize imageSize = render.imageSize();
            this->_font.loadFromFile("assets/arial.ttf");

            this->_image.create(imageSize.width, imageSize.heigth,
                                sf::Color::Black);
            std::chrono::time_point last = std::chrono::steady_clock::now();

            while (!render.renderingIsFinished() && this->_window.isOpen()) {
                std::chrono::time_point now = std::chrono::steady_clock::now();
                double elapsed =
                    std::chrono::duration<double>(now - last).count();

                if (elapsed >= 1.0) {
                    this->_window.clear();
                    this->updateWindow(render, imageSize);
                    this->_window.display();
                    last = now;
                }
                this->eventHandling(render);
            }
            this->updateWindow(render, imageSize);
            while (this->_window.isOpen() && !this->fullRender()) {
                this->eventHandling(render);
            }
            this->_window.setActive(false);
        });
    }

    void SFMLVisual::updateWindowSize() {
        sf::Vector2u windowSize = this->_window.getSize();
        this->_windowSize = sf::Vector2f(static_cast<float>(windowSize.x),
                                         static_cast<float>(windowSize.y));
    }

    void SFMLVisual::updateWindow(Render &render, ImageSize &imageSize) {
        this->updateWindowSize();

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
        for (int y = 0; y < size.heigth; y++) {
            for (int x = 0; x < size.width; x++) {
                maths::Color &color = pixels.at(y * size.width + x);

                if ((color.r == 0 && color.g == 0 && color.b == 0))
                    continue;
                this->_image.setPixel(x, y,
                                      sf::Color(color.r, color.g, color.b));
            }
        }
        sf::Texture texture;
        sf::Sprite sprite;

        texture.loadFromImage(this->_image);
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

    void SFMLVisual::keyPressHandling(sf::Event &event, Render &render) {
        switch (event.key.code) {
            case sf::Keyboard::F:
                this->_fullRender = true;
                render.stopRendering();
            case sf::Keyboard::S:
                if (render.renderingIsFinished() && this->_fullRender) {
                    this->_save = true;
                }

            default:
                break;
        }
    }

    void SFMLVisual::eventHandling(Render &render) {
        sf::Event event;

        while (this->_window.pollEvent(event)) {
            if (event.type == sf::Event::Resized) {
                this->_window.setView(sf::View(
                    sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
            if (event.type == sf::Event::Closed) {
                render.stopRendering();
                this->_window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                this->keyPressHandling(event, render);
            }
        }
    }

}  // namespace raytracer::visual
