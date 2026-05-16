/*
** EPITECH PROJECT, 2026
** SFMLPage
** File description:
** SFMLPage header
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <functional>
#include <string>

#include "Render.hpp"
#include "SFMLContext.hpp"

namespace raytracer::visual {

    class SFMLPage {
      public:
        explicit SFMLPage(SFMLContext &ctx) : _ctx(ctx) {
        }
        virtual ~SFMLPage() = default;

        /**
         * @brief Check if a stop/exit request has been made by the user.
         * @return true if stop has been requested, false otherwise.
         */
        bool stopRequested() const {
            return _stop;
        }

      protected:
        SFMLContext &_ctx;
        bool _stop = false;

        /**
         * @brief Display text at a specified position on the screen.
         * @param posX The X coordinate for text placement.
         * @param posY The Y coordinate for text placement.
         * @param str The text string to display.
         * @param size The font size for the text (default: 40).
         */
        void displayText(float posX, float posY, const std::string &str,
                         unsigned int size = 40);

        /**
         * @brief Check for and handle SFML window events.
         * @param render The render context for event processing.
         * @param onEvent Callback function to handle individual events.
         */
        void checkEvents(Render &render,
                         const std::function<void(sf::Event &)> &onEvent);

        /**
         * @brief Run the main event/render loop for the page.
         * @param render The render context for the loop.
         * @param stop Callback function to check if the loop should stop.
         * @param draw Callback function to perform drawing operations each
         * frame.
         * @param onEvent Callback function to handle SFML window events.
         */
        void runLoop(Render &render, const std::function<bool()> &stop,
                     const std::function<void()> &draw,
                     const std::function<void(sf::Event &)> &onEvent);
    };
}  // namespace raytracer::visual
