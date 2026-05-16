/*
** EPITECH PROJECT, 2026
** SFMLImagePage
** File description:
** SFMLImagePage code
*/

#include "SFMLImagePage.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

namespace raytracer::visual {

    void SFMLImagePage::reset() {
        _fullRender = false;
        _save = false;
        _goBack = false;
        _showSaveButton = false;
        _cachedPreviewPixels.clear();
    }

    std::thread SFMLImagePage::printProgress(Render &render) {
        return std::thread([this, &render]() {
            _ctx.window().setActive(true);
            ImageSize imageSize = render.imageSize();

            auto onEvent = [this, &render](sf::Event &e) {
                handleEvent(e, render);
            };
            auto draw = [this, &render, &imageSize]() {
                drawFrame(render, imageSize);
            };

            runLoop(
                render,
                [this, &render]() {
                    return !isActive() || render.renderingIsFinished();
                },
                draw, onEvent);

            if (_ctx.window().isOpen()) {
                _ctx.window().clear();
                draw();
                _ctx.window().display();
            }

            runLoop(
                render, [this]() { return !isActive() || _fullRender; }, draw,
                onEvent);

            if (_fullRender && _cachedPreviewPixels.empty()) {
                _cachedPreviewPixels = render.pixels();
                _cacheImageSize = imageSize;
            }
            _ctx.window().setActive(false);
        });
    }

    bool SFMLImagePage::wantSave(Render &render) {
        _ctx.window().setActive(true);
        ImageSize imageSize = render.imageSize();
        _showSaveButton = true;

        runLoop(
            render, [this]() { return !isActive() || _save; },
            [this, &render, &imageSize]() { drawFrame(render, imageSize); },
            [this, &render](sf::Event &e) { handleEvent(e, render); });

        _showSaveButton = false;
        _ctx.window().setActive(false);
        return _save;
    }

    void SFMLImagePage::drawFrame(Render &render, ImageSize &imageSize) {
        _ctx.updateWindowSize();

        if (!_cachedPreviewPixels.empty())
            drawPixels(_cachedPreviewPixels, _cacheImageSize);

        auto pixels = render.pixels();
        drawPixels(pixels, imageSize);

        displayText(_ctx.windowSize().x * 0.5f, _ctx.windowSize().y * 0.07f,
                    _fullRender ? "Full Render Mode" : "Preview Mode");

        const auto &ws = _ctx.windowSize();
        float btnW = std::max(120.f, ws.x * 0.4f);
        float btnH = std::max(30.f, ws.y * 0.06f);
        float btnY = ws.y * 0.92f - btnH / 2.f;

        if (!_fullRender) {
            _fullRenderButton =
                sf::FloatRect((ws.x - btnW) / 2.f, btnY, btnW, btnH);
            drawButton(_fullRenderButton, "Full Render (F)");
        } else if (_showSaveButton) {
            _saveButton = sf::FloatRect((ws.x - btnW) / 2.f, btnY, btnW, btnH);
            drawButton(_saveButton, "Save (S)");
        }
    }

    void SFMLImagePage::drawPixels(std::vector<maths::Color> &pixels,
                                   ImageSize &size) {
        sf::Image image;
        image.create(size.width, size.heigth, sf::Color::Transparent);
        for (int y = 0; y < size.heigth; y++) {
            for (int x = 0; x < size.width; x++) {
                maths::Color &c = pixels.at(y * size.width + x);
                if (c.r == 0 && c.g == 0 && c.b == 0)
                    continue;
                image.setPixel(x, y, sf::Color(c.r, c.g, c.b));
            }
        }
        sf::Texture texture;
        sf::Sprite sprite;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        layoutSprite(size, sprite);
        _ctx.window().draw(sprite);
    }

    void SFMLImagePage::layoutSprite(ImageSize &size, sf::Sprite &sprite) {
        const auto &ws = _ctx.windowSize();
        float aw = std::max(0.f, ws.x * (1.0f - _marginLeft - _marginRight));
        float ah = std::max(0.f, ws.y * (1.0f - _marginTop - _marginBottom));
        float sx = aw / static_cast<float>(size.width);
        float sy = ah / static_cast<float>(size.heigth);
        float scale = std::max(0.f, std::min(sx, sy));

        if (scale <= 0.f)
            return;

        float dw = size.width * scale;
        float dh = size.heigth * scale;
        float px = ws.x * _marginLeft + (aw - dw) / 2.f;
        float py = ws.y * _marginTop + (ah - dh) / 2.f;
        sprite.setScale(scale, scale);
        sprite.setPosition(px, py);
    }

    void SFMLImagePage::drawButton(const sf::FloatRect &rect,
                                   const std::string &label) {
        sf::RectangleShape box(sf::Vector2f(rect.width, rect.height));
        box.setPosition(rect.left, rect.top);

        // Hover effect
        sf::Vector2i mousePos = sf::Mouse::getPosition(_ctx.window());
        bool hovered = isMouseOver(rect, mousePos.x, mousePos.y);

        box.setFillColor(hovered ? sf::Color(80, 80, 120)
                                 : sf::Color(50, 50, 80));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        _ctx.window().draw(box);

        displayText(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f,
                    label);
    }

    bool SFMLImagePage::isMouseOver(const sf::FloatRect &rect, int mx,
                                    int my) const {
        return rect.contains(static_cast<float>(mx), static_cast<float>(my));
    }

    void SFMLImagePage::handleEvent(sf::Event &event, Render &render) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            int mx = event.mouseButton.x;
            int my = event.mouseButton.y;

            if (!_fullRender && isMouseOver(_fullRenderButton, mx, my)) {
                _fullRender = true;
                render.stopRendering();
                return;
            }
            if (_showSaveButton && render.renderingIsFinished() &&
                _fullRender && isMouseOver(_saveButton, mx, my)) {
                _save = true;
                return;
            }
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Backspace) {
            _goBack = true;
            render.stopRendering();
        }
    }
}  // namespace raytracer::visual
