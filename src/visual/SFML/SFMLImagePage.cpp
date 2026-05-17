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
                    return !this->isActive() || render.renderingIsFinished() ||
                           render.renderedStopped();
                },
                draw, onEvent);

            if (this->_ctx.window().isOpen()) {
                this->_ctx.window().clear();
                draw();
                this->_ctx.window().display();
            }
            this->runLoop(
                render,
                [this, &render]() {
                    return !this->isActive() || this->_fullRender ||
                           render.reloadRequested();
                },
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

        const auto windowSize = this->_ctx.windowSize();
        float btnW = std::max(120.f, windowSize.x * 0.4f);
        float btnH = std::max(30.f, windowSize.y * 0.06f);
        float btnY = windowSize.y * 0.92f - btnH / 2.f;

        float backW = std::max(80.f, windowSize.x * 0.08f);
        float backH = std::max(30.f, windowSize.y * 0.05f);
        float backMargin = std::max(10.f, windowSize.x * 0.01f);
        this->_backButton = sf::FloatRect(backMargin, backMargin, backW, backH);
        this->drawButton(this->_backButton, "<");

        if (!this->_fullRender) {
            this->_fullRenderButton =
                sf::FloatRect((windowSize.x - btnW) / 2.f, btnY, btnW, btnH);
            this->drawButton(this->_fullRenderButton, "Full Render");
        } else if (this->_showSaveButton) {
            this->_saveButton =
                sf::FloatRect((windowSize.x - btnW) / 2.f, btnY, btnW, btnH);
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
        const auto windowSize = this->_ctx.windowSize();

        const float headerH = std::max(40.f, windowSize.y * 0.10f);
        const float footerH = std::max(60.f, windowSize.y * 0.12f);
        const float sideMargin = std::max(20.f, windowSize.x * 0.05f);

        float avalaibleWidth = std::max(0.f, windowSize.x - 2.f * sideMargin);
        float avalaibleHeight =
            std::max(0.f, windowSize.y - headerH - footerH - 2.f);
        if (avalaibleWidth <= 0.f || avalaibleHeight <= 0.f)
            return;

        float scale =
            std::min(avalaibleWidth / static_cast<float>(size.width),
                     avalaibleHeight / static_cast<float>(size.heigth));
        if (scale <= 0.f)
            return;

        float displayWidth = size.width * scale;
        float displayHeight = size.heigth * scale;
        float x = sideMargin + (avalaibleWidth - displayWidth) / 2.f;
        float y = headerH + (avalaibleHeight - displayHeight) / 2.f;

        sprite.setScale(scale, scale);
        sprite.setPosition(x, y);
    }

    void SFMLImagePage::handleEvent(sf::Event &event, Render &render) {
        if (event.type != sf::Event::MouseButtonPressed ||
            event.mouseButton.button != sf::Mouse::Left)
            return;
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;

        if (this->isMouseOver(this->_backButton, mouseX, mouseY)) {
            this->_goBack = true;
            render.stopRendering();
            return;
        }
        if (!this->_fullRender &&
            this->isMouseOver(this->_fullRenderButton, mouseX, mouseY)) {
            this->_fullRender = true;
            render.stopRendering();
            return;
        }
        if (this->_showSaveButton && render.renderingIsFinished() &&
            this->_fullRender &&
            this->isMouseOver(this->_saveButton, mouseX, mouseY)) {
            this->_save = true;
            return;
        }
    }
}  // namespace raytracer::visual
