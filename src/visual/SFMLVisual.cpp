/*
** EPITECH PROJECT, 2026
** SFMLVisual
** File description:
** SFMLVisual code
*/

#include "visual/SFMLVisual.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
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

    std::thread SFMLVisual::printProgress(int activeWorkers, Render &render) {
        return std::thread([this, &render, activeWorkers]() {
            this->_window.setActive(true);
            ImageSize size = render.imageSize();

            this->_image.create(size.width, size.heigth, sf::Color::Black);
            std::chrono::time_point last = std::chrono::steady_clock::now();

            while (!render.renderingIsFinished() && this->_window.isOpen()) {
                std::chrono::time_point now = std::chrono::steady_clock::now();
                double elapsed =
                    std::chrono::duration<double>(now - last).count();

                if (elapsed >= 1.0) {
                    this->_window.clear();
                    this->dispayPixels(render.pixels(), size);

                    this->_window.display();

                    last = now;
                }
                this->eventHandling(render);
            }
            this->_window.close();
            this->_window.setActive(false);
        });
    }

    void SFMLVisual::dispayPixels(std::vector<maths::Color> &pixels,
                                  ImageSize size) {
        for (int y = 0; y < size.heigth; y++) {
            for (int x = 0; x < size.width; x++) {
                maths::Color &color = pixels.at(y * size.width + x);
                if ((color.r == 0 && color.g == 0 && color.b == 0)) {
                    continue;
                }
                this->_image.setPixel(x, y,
                                      sf::Color(color.r, color.g, color.b));
            }
        }
        this->_texture.loadFromImage(this->_image);
        sf::Sprite sprite(this->_texture);

        sf::Vector2u windowSize = this->_window.getSize();

        float scaleX =
            static_cast<float>(windowSize.x) / static_cast<float>(size.width);
        float scaleY =
            static_cast<float>(windowSize.y) / static_cast<float>(size.heigth);

        sprite.setScale(scaleX, scaleY);
        sprite.setPosition(0, 0);

        this->_window.draw(sprite);
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
            }
        }
    }
}  // namespace raytracer::visual
