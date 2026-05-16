/*
** EPITECH PROJECT, 2026
** SFMLImagePage
** File description:
** SFMLImagePage code
*/

#include "SFMLImagePage.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

namespace raytracer::visual {

    void SFMLImagePage::reset() {
        _fullRender = false;
        _save = false;
        _goBack = false;
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

        runLoop(
            render, [this]() { return !isActive() || _save; },
            [this, &render, &imageSize]() {
                drawFrame(render, imageSize);
                displayText(_ctx.windowSize().x * 0.5f,
                            _ctx.windowSize().y * 0.9f,
                            "Click on \"S\" to save this file");
            },
            [this, &render](sf::Event &e) { handleEvent(e, render); });

        _ctx.window().setActive(false);
        return _save;
    }

    void SFMLImagePage::drawFrame(Render &render, ImageSize &imageSize) {
        _ctx.updateWindowSize();

        if (!_cachedPreviewPixels.empty())
            drawPixels(_cachedPreviewPixels, _cacheImageSize);

        auto pixels = render.pixels();
        drawPixels(pixels, imageSize);

        displayText(_ctx.windowSize().x * 0.5f, _ctx.windowSize().y * 0.1f,
                    _fullRender ? "Full Render Mode" : "preview Mode");
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

    void SFMLImagePage::handleEvent(sf::Event &event, Render &render) {
        if (event.type != sf::Event::KeyPressed)
            return;
        switch (event.key.code) {
            case sf::Keyboard::Backspace:
                _goBack = true;
                render.stopRendering();
                break;
            case sf::Keyboard::F:
                _fullRender = true;
                render.stopRendering();
                break;
            case sf::Keyboard::S:
                if (render.renderingIsFinished() && _fullRender)
                    _save = true;
                break;
            default:
                break;
        }
    }
}  // namespace raytracer::visual
