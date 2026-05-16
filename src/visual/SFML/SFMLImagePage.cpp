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
        this->_fullRender = false;
        this->_save = false;
        this->_goBack = false;
        this->_showSaveButton = false;
        this->_cachedPreviewPixels.clear();
    }

    std::thread SFMLImagePage::printProgress(Render &render) {
        return std::thread([this, &render]() {
            this->_ctx.window().setActive(true);
            ImageSize imageSize = render.imageSize();

            auto onEvent = [this, &render](sf::Event &e) {
                this->handleEvent(e, render);
            };
            auto draw = [this, &render, &imageSize]() {
                this->drawFrame(render, imageSize);
            };

            this->runLoop(
                render,
                [this, &render]() {
                    return !this->isActive() || render.renderingIsFinished();
                },
                draw, onEvent);

            if (this->_ctx.window().isOpen()) {
                this->_ctx.window().clear();
                draw();
                this->_ctx.window().display();
            }

            this->runLoop(
                render,
                [this]() { return !this->isActive() || this->_fullRender; },
                draw, onEvent);

            if (this->_fullRender && this->_cachedPreviewPixels.empty()) {
                this->_cachedPreviewPixels = render.pixels();
                this->_cacheImageSize = imageSize;
            }
            this->_ctx.window().setActive(false);
        });
    }

    bool SFMLImagePage::wantSave(Render &render) {
        this->_ctx.window().setActive(true);
        ImageSize imageSize = render.imageSize();
        this->_showSaveButton = true;

        this->runLoop(
            render, [this]() { return !this->isActive() || this->_save; },
            [this, &render, &imageSize]() {
                this->drawFrame(render, imageSize);
            },
            [this, &render](sf::Event &e) { this->handleEvent(e, render); });

        this->_showSaveButton = false;
        this->_ctx.window().setActive(false);
        return this->_save;
    }

    void SFMLImagePage::drawFrame(Render &render, ImageSize &imageSize) {
        this->_ctx.updateWindowSize();

        if (!this->_cachedPreviewPixels.empty())
            this->drawPixels(this->_cachedPreviewPixels, this->_cacheImageSize);

        auto pixels = render.pixels();
        this->drawPixels(pixels, imageSize);

        this->displayText(
            this->_ctx.windowSize().x * 0.5f, this->_ctx.windowSize().y * 0.07f,
            this->_fullRender ? "Full Render Mode" : "Preview Mode");

        const auto &ws = this->_ctx.windowSize();
        float btnW = std::max(120.f, ws.x * 0.4f);
        float btnH = std::max(30.f, ws.y * 0.06f);
        float btnY = ws.y * 0.92f - btnH / 2.f;

        this->drawButton(this->_fullRenderButton, "Full Render");

        if (!this->_fullRender) {
            this->_fullRenderButton =
                sf::FloatRect((ws.x - btnW) / 2.f, btnY, btnW, btnH);
            this->drawButton(this->_fullRenderButton, "Full Render");
        } else if (this->_showSaveButton) {
            this->_saveButton =
                sf::FloatRect((ws.x - btnW) / 2.f, btnY, btnW, btnH);
            this->drawButton(this->_saveButton, "Save");
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
        const auto &ws = this->_ctx.windowSize();

        const float headerH = std::max(40.f, ws.y * 0.10f);
        const float footerH = std::max(60.f, ws.y * 0.12f);
        const float sideM = std::max(20.f, ws.x * 0.05f);

        float aw = std::max(0.f, ws.x - 2.f * sideM);
        float ah = std::max(0.f, ws.y - headerH - footerH - 2.f);
        if (aw <= 0.f || ah <= 0.f)
            return;

        float sx = aw / static_cast<float>(size.width);
        float sy = ah / static_cast<float>(size.heigth);
        float scale = std::min(sx, sy);
        if (scale <= 0.f)
            return;

        float dw = size.width * scale;
        float dh = size.heigth * scale;
        float px = sideM + (aw - dw) / 2.f;
        float py = headerH + (ah - dh) / 2.f;

        sprite.setScale(scale, scale);
        sprite.setPosition(px, py);
    }
    void SFMLImagePage::drawButton(const sf::FloatRect &rect,
                                   const std::string &label) {
        sf::RectangleShape box(sf::Vector2f(rect.width, rect.height));
        box.setPosition(rect.left, rect.top);

        // Hover effect
        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_ctx.window());
        bool hovered = this->isMouseOver(rect, mousePos.x, mousePos.y);

        box.setFillColor(hovered ? sf::Color(80, 80, 120)
                                 : sf::Color(50, 50, 80));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        this->_ctx.window().draw(box);

        this->displayText(rect.left + rect.width / 2.f,
                          rect.top + rect.height / 2.f, label);
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

            if (!this->_fullRender &&
                this->isMouseOver(this->_fullRenderButton, mx, my)) {
                this->_fullRender = true;
                render.stopRendering();
                return;
            }
            if (this->_showSaveButton && render.renderingIsFinished() &&
                this->_fullRender &&
                this->isMouseOver(this->_saveButton, mx, my)) {
                this->_save = true;
                return;
            }
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Backspace) {
            this->_goBack = true;
            render.stopRendering();
        }
    }
}  // namespace raytracer::visual
