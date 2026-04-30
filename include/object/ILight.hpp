/*
** EPITECH PROJECT, 2026
** ILight
** File description:
** ILight header
*/

#pragma once

#include "IScene.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::light {

    class ILight {
      public:
        virtual ~ILight() = default;

        virtual maths::Vector computeNEE(const scene::IScene &scene,
                                         const maths::Vector &x,
                                         const maths::Vector &nl,
                                         const maths::Vector &f) const = 0;
        virtual void setPosition(const maths::Vector &position) = 0;
        virtual void setColor(const maths::Color &color) = 0;
        virtual void setIntensity(const double intensity) = 0;

        virtual maths::Vector position() const = 0;
        virtual maths::Color color() const = 0;
        virtual double intensity() const = 0;
    };

}  // namespace raytracer::object::light
