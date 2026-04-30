/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** IScene
*/

#ifndef ISCENE_HPP_
#define ISCENE_HPP_

#include "math/Ray.hpp"

namespace raytracer::object::scene {
    class IScene {
      public:
        IScene() = default;
        ~IScene() = default;

        virtual bool intersect(const maths::Ray &ray, double &t,
                               int &objectId) const = 0;

      protected:
      private:
    };
}  // namespace raytracer::object::scene
#endif /* !ISCENE_HPP_ */
