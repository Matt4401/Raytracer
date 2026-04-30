/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ALight
*/

#include "object/ALight.hpp"

namespace raytracer::object::light {

    void ALight::setPosition(const maths::Vector &position) {
        this->_position = position;
    }

    void ALight::setColor(const maths::Color &color) {
        this->_color = color;
    }

    void ALight::setIntensity(const double intensity) {
        this->_intensity = intensity;
    }

    maths::Vector ALight::position() const {
        return this->_position;
    }

    maths::Color ALight::color() const {
        return this->_color;
    }

    double ALight::intensity() const {
        return this->_intensity;
    }
}  // namespace raytracer::object::light
