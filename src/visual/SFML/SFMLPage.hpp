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

        bool stopRequested() const {
            return _stop;
        }

      protected:
        SFMLContext &_ctx;
        bool _stop = false;

        void displayText(float posX, float posY, const std::string &str,
                         unsigned int size = 40);

        // Pumps events; handles resize/close globally then delegates
        void checkEvents(Render &render,
                        const std::function<void(sf::Event &)> &onEvent);
    };
}  // namespace raytracer::visual
