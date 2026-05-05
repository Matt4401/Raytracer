/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** IScene
*/

#pragma once

#include <memory>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/IObject.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::light {
    class ILight;
}  // namespace raytracer::object::light

namespace raytracer::object::camera {
    class ICamera;
}  // namespace raytracer::object::camera

namespace raytracer::object::scene {

    struct AmbientDiffuse {
        maths::Color ambient;
        double intensity;
    };

    struct AmbientLight {
        maths::Color color;
        double intensity;
    };

    struct AmbientOcclusion {
        int samples;
        double radius;
    };

    class IScene {
      public:
        IScene() = default;
        virtual ~IScene() = default;

        virtual bool intersect(const maths::Ray &ray, double &t,
                               int &objectId) const = 0;
        virtual maths::Vector radiance(const maths::Ray &ray, int depth,
                                       unsigned short *xi,
                                       int emissive = 1) const = 0;

        virtual void addObject(std::shared_ptr<IObject> object) = 0;
        virtual void setAmbientLight(const maths::Color &color,
                                     double intensity) = 0;
        virtual void setDiffuseLight(const maths::Color &color,
                                     double intensity) = 0;
        virtual void setAmbientOcclusion(int samples, double radius) = 0;

        virtual AmbientOcclusion ambientOcclusion() const = 0;
        virtual AmbientLight ambientLight() const = 0;
        virtual AmbientDiffuse ambientDiffuse() const = 0;

        virtual const std::vector<std::shared_ptr<primitive::IPrimitive>> &
        primitives() const = 0;
        virtual const std::vector<std::shared_ptr<light::ILight>> &lights()
            const = 0;
        virtual const std::vector<std::shared_ptr<camera::ICamera>> &cameras()
            const = 0;

      protected:
      private:
    };
}  // namespace raytracer::object::scene
