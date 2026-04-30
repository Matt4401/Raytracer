/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** DirectionalLight
*/

#ifndef DIRECTIONALLIGHT_HPP_
#define DIRECTIONALLIGHT_HPP_

#include <any>
#include <map>
#include <string>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/ALight.hpp"
#include "object/IScene.hpp"

namespace raytracer::object::light {
    class DirectionalLight : public ALight {
      public:
        DirectionalLight(const std::map<std::string, std::any> &params);
        ~DirectionalLight() override = default;

        /// @brief Compute the NEE contribution from this directional light.
        /// @param scene the scene containing geometry for visibility testing
        /// @param x the surface position
        /// @param nl the surface normal (oriented towards the hemisphere)
        /// @param f the surface color / BRDF (albedo for diffuse surfaces)
        /// @return the contribution from this light
        maths::Vector computeNEE(const scene::IScene &scene,
                                 const maths::Vector &x,
                                 const maths::Vector &nl,
                                 const maths::Vector &f) const override;

      protected:
      private:
        maths::Vector _direction;
    };
}  // namespace raytracer::object::light

#endif /* !DIRECTIONALLIGHT_HPP_ */
