/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ALight
*/

#include "object/ALight.hpp"

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::light {

    ALight::ALight(const std::map<std::string, std::any> &params)
        : AObject(Type::LIGHT) {
        const double intensity = util::ObjectMiddleware::validate<double>(
            params, "intensity", "ALight");
        util::Helpers::unsignedDouble(intensity, "intensity", "ALight");

        _intensity = intensity;
        _position = util::Helpers::toVector(params, "position", "ALight");
        _color = util::Helpers::toColor(params, "color", "ALight");
    }

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
