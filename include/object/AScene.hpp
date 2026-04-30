/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AScene
*/

#ifndef ASCENE_HPP_
#define ASCENE_HPP_

#include <any>
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "AObject.hpp"
#include "ICamera.hpp"
#include "ILight.hpp"
#include "IPrimitive.hpp"
#include "IScene.hpp"

namespace raytracer::object::scene {
    class AScene : public IScene, public AObject {
      public:
        AScene() : AObject(Type::SCENE) {};
        explicit AScene(const std::map<std::string, std::any> &params);
        ~AScene() override = default;

        virtual bool intersect(const maths::Ray &ray, double &t,
                               int &objectId) const = 0;
        virtual maths::Vector radiance(const maths::Ray &ray, int depth,
                                       unsigned short *Xi,
                                       int emissive = 1) = 0;

        void addObject(const IObject &object) override;
        virtual void setAmbientLight(const maths::Color &color,
                                     double intensity) = 0;
        virtual void setDiffuseLight(const maths::Color &color,
                                     double intensity) = 0;
        virtual void setAmbientOcclusion(int samples, double radius) = 0;

        virtual AmbiantOcclusion ambiantOcclusion() const = 0;
        virtual AmbientLight ambientLight() const = 0;
        virtual AmbientDiffuse ambientDiffuse() const = 0;

      protected:
        std::vector<std::unique_ptr<primitive::IPrimitive>> _primitives;
        std::vector<std::unique_ptr<light::ILight>> _lights;
        std::vector<std::unique_ptr<camera::ICamera>> _cameras;

        AmbiantOcclusion _ambiantOcclusion;
        AmbientLight _ambientLight;
        AmbientDiffuse _ambientDiffuse;

      private:
        void addPrimitive(const object::IObject &primitive);
        void addLight(const object::IObject &light);
        void addCamera(const object::IObject &camera);
        std::map<object::IObject::Type, std::function<void(const IObject &)>>
            _addObjectHandlers = {
                {object::IObject::Type::PRIMITIVE,
                 [this](const IObject &obj) { addPrimitive(obj); }},
                {object::IObject::Type::LIGHT,
                 [this](const IObject &obj) { addLight(obj); }},
                {object::IObject::Type::CAMERA,
                 [this](const IObject &obj) { addCamera(obj); }},
            };
    };
}  // namespace raytracer::object::scene

#endif /* !ASCENE_HPP_ */
