/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ALight
*/

#include "object/ALight.hpp"

#include <algorithm>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::light {

    ALight::ALight()
        : AObject(Type::LIGHT), _position(), _color(), _intensity(0.0) {
    }

    ALight::ALight(const std::map<std::string, std::any> &params)
        : AObject(Type::LIGHT) {
        const double intensity = util::ObjectMiddleware::validate<double>(
            params, "intensity", "ALight");
        util::Helpers::unsignedDouble(intensity, "intensity", "ALight");

        _intensity = intensity;
        _position = util::Helpers::toVector(params, "position", "ALight");
        _color = util::Helpers::toColor(params, "color", "ALight");
    }

    ALight::ALight(maths::Vector position, maths::Color color, double intensity)
        : AObject(Type::LIGHT),
          _position(position),
          _color(color),
          _intensity(intensity) {
    }

    ALight::ALight(Type type, maths::Vector position, maths::Color color,
                   double intensity)
        : AObject(type),
          _position(position),
          _color(color),
          _intensity(intensity) {
    }

    maths::Vector ALight::materialDiffuseResponse(
        const primitive::MaterialProperties &material) {
        const double reflectivity = std::clamp(material.reflectivity, 0.0, 1.0);
        const double transparency = std::clamp(material.transparency, 0.0, 1.0);
        const double metalness = std::clamp(material.metalness, 0.0, 1.0);
        return material.color.toVector() * (1.0 - reflectivity) *
               (1.0 - transparency) * (1.0 - metalness);
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
