/*
** EPITECH PROJECT, 2026
** ILight
** File description:
** ILight header
*/

#pragma once

#include "math/Color.hpp"
#include "math/Vector.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::scene {
    class IScene;
}  // namespace raytracer::object::scene

namespace raytracer::object::light {

    class ILight {
      public:
        virtual ~ILight() = default;

        /// @brief Compute the NEE contribution from this spot light.
        /// @param scene the scene containing geometry for visibility testing
        /// @param x the surface position
        /// @param nl the surface normal (oriented towards the hemisphere)
        /// @param f the surface color / BRDF
        /// @return the contribution from this light
        virtual maths::Vector computeNEE(const object::scene::IScene &scene,
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
