/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ALight
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "AObject.hpp"
#include "ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::light {
    class ALight : public ILight, public object::AObject {
      public:
        ALight()
            : AObject(Type::LIGHT), _position(), _color(), _intensity(0.0) {
        }
        explicit ALight(const std::map<std::string, std::any> &params);
        explicit ALight(maths::Vector position, maths::Color color,
                        double intensity)
            : AObject(Type::LIGHT),
              _position(position),
              _color(color),
              _intensity(intensity) {
        }
        explicit ALight(Type type, maths::Vector position, maths::Color color,
                        double intensity)
            : AObject(type),
              _position(position),
              _color(color),
              _intensity(intensity) {
        }
        ~ALight() override = default;

        void setPosition(const maths::Vector &position) override;
        void setColor(const maths::Color &color) override;
        void setIntensity(const double intensity) override;

        maths::Vector position() const override;
        maths::Color color() const override;
        double intensity() const override;

      protected:
        static constexpr double K_MIN_DISTANCE_EPSILON = 1e-6;
        static constexpr double K_SHADOW_RAY_BIAS = 1e-4;
        static constexpr double K_OCCLUSION_EPSILON = 1e-3;

        maths::Vector _position;
        maths::Color _color;
        double _intensity;

      private:
    };
}  // namespace raytracer::object::light
