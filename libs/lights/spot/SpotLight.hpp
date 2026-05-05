/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpotLight
*/

#pragma once

#include <any>
#include <map>
#include <string>

#include "math/Vector.hpp"
#include "object/ALight.hpp"
#include "object/IScene.hpp"

namespace raytracer::object::light {
    class SpotLight : public ALight {
      public:
        explicit SpotLight(const std::map<std::string, std::any> &params);
        ~SpotLight() override = default;

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
        maths::Vector _direction;
        double _exponent;
    };
}  // namespace raytracer::object::light
