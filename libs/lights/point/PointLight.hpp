/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** PointLight
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "object/ALight.hpp"
#include "object/IScene.hpp"

namespace raytracer::object::light {
    class PointLight : public ALight {
      public:
        PointLight()
            : ALight(maths::Vector(), maths::Color(), 0.0), _radius(0.0) {
        }

        explicit PointLight(const std::map<std::string, std::any> &params);

        /// @brief Construct a point light with the given position, color, and
        /// intensity.
        /// @param pos the position of the light in 3D space.
        /// @param col the color of the light.
        /// @param intens the intensity of the light.
        /// @param rad the radius of the light for area light calculations. A
        /// value of 0.0 will treat the light as a point light with
        /// inverse-square attenuation, while a positive value will treat the
        /// light as an area light, where the intensity is distributed over a
        /// sphere of the given radius.
        PointLight(const maths::Vector &pos, const maths::Color &col,
                   double intens, double rad = 0.0)
            : ALight(maths::Vector(pos), maths::Color(col), intens),
              _radius(rad) {
        }

        /// @brief Compute the NEE contribution from this point light.
        /// @param scene the scene containing geometry for visibility testing
        /// @param x the surface position
        /// @param nl the surface normal (oriented towards the hemisphere)
        /// @param f the surface color / BRDF
        /// @return the contribution from this light
        maths::Vector computeNEE(const object::scene::IScene &scene,
                                 const maths::Vector &x,
                                 const maths::Vector &nl,
                                 const maths::Vector &f) const override;

      protected:
      private:
        double _radius;
    };
}  // namespace raytracer::object::light
