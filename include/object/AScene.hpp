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
#include "IScene.hpp"
#include "primitive/IPrimitive.hpp"

namespace raytracer::object::scene {
    class AScene : public IScene, public AObject {
      public:
        AScene() : AObject(Type::SCENE) {};
        explicit AScene(const std::map<std::string, std::any> &params);
        ~AScene() override = default;

        virtual bool intersect(const maths::Ray &ray, double &t,
                               int &objectId) const override = 0;
        virtual maths::Vector radiance(const maths::Ray &ray, int depth,
                                       unsigned short *Xi,
                                       int emissive = 1) override = 0;

        void addObject(IObject &object) override;
        void setAmbientLight(const maths::Color &color,
                             double intensity) override;
        void setDiffuseLight(const maths::Color &color,
                             double intensity) override;
        void setAmbientOcclusion(int samples, double radius) override;

        AmbiantOcclusion ambiantOcclusion() const override;
        AmbientLight ambientLight() const override;
        AmbientDiffuse ambientDiffuse() const override;

      protected:
        static constexpr int kMaxRadianceDepth = 10;
        static constexpr int kDiffuseRussianRouletteDepth = 5;
        static constexpr int kRefractiveRussianRouletteDepth = 2;
        static constexpr double kColorScale = 1.0 / 255.0;
        static constexpr double kOnbAxisThreshold = 0.1;
        static constexpr double kDefaultIor = 1.5;

        std::vector<std::unique_ptr<primitive::IPrimitive>> _primitives;
        std::vector<std::unique_ptr<light::ILight>> _lights;
        std::vector<std::unique_ptr<camera::ICamera>> _cameras;

        AmbiantOcclusion _ambiantOcclusion;
        AmbientLight _ambientLight;
        AmbientDiffuse _ambientDiffuse;

      private:
        void addPrimitive(object::AObject *primitive);
        void addLight(object::AObject *light);
        void addCamera(object::AObject *camera);
        std::map<object::IObject::Type, std::function<void(AObject *)>>
            _addObjectHandlers = {
                {object::IObject::Type::PRIMITIVE,
                 [this](AObject *obj) { addPrimitive(obj); }},
                {object::IObject::Type::LIGHT,
                 [this](AObject *obj) { addLight(obj); }},
                {object::IObject::Type::CAMERA,
                 [this](AObject *obj) { addCamera(obj); }},
            };
    };
}  // namespace raytracer::object::scene

#endif /* !ASCENE_HPP_ */
