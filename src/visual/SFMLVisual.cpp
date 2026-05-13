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
        this->_window.create(sf::VideoMode(800, 600), "Raytracer SFML",
                             sf::Style::Titlebar | sf::Style::Close);

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
            }
            this->_window.setActive(false);
        }

        );
    }

    void SFMLVisual::dispayPixels(const std::vector<maths::Color> &pixels,
                                  ImageSize size) {
        for (int y = 0; y < size.heigth; y++) {
            for (int x = 0; x < size.width; x++) {
                maths::Color color = pixels.at(y * size.width + x);
                this->_image.setPixel(
                    x, y,
                    sf::Color(color.r, color.g, color.b));  // r, g, b: 0-255
            }
        }
        this->_texture.loadFromImage(this->_image);
        sf::Sprite sprite(this->_texture);

        sf::Vector2u windowSize = this->_window.getSize();
        sf::Vector2u imageSize = this->_image.getSize();

        float posX = (windowSize.x - imageSize.x) / 3.0f;
        float posY = (windowSize.y - imageSize.y) / 3.0f;

        sprite.setPosition(posX, posY);
        sprite.scale(1.5, 1.5);

        this->_window.draw(sprite);
    }
}  // namespace raytracer::visual
