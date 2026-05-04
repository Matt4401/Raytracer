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
                                       int emissive = 1) const override = 0;

        void addObject(std::shared_ptr<IObject> object) override;
        void setAmbientLight(const maths::Color &color,
                             double intensity) override;
        void setDiffuseLight(const maths::Color &color,
                             double intensity) override;
        void setAmbientOcclusion(int samples, double radius) override;

        AmbientOcclusion ambientOcclusion() const override;
        AmbientLight ambientLight() const override;
        AmbientDiffuse ambientDiffuse() const override;

        const std::vector<std::shared_ptr<primitive::IPrimitive>> &primitives()
            const override;
        const std::vector<std::shared_ptr<light::ILight>> &lights()
            const override;
        const std::vector<std::shared_ptr<camera::ICamera>> &cameras()
            const override;

      protected:
        static constexpr int kMaxRadianceDepth = 10;
        static constexpr int kDiffuseRussianRouletteDepth = 5;
        static constexpr int kRefractiveRussianRouletteDepth = 2;
        static constexpr double kColorScale = 1.0 / 255.0;
        static constexpr double kOnbAxisThreshold = 0.1;
        static constexpr double kDefaultIor = 1.5;

        std::vector<std::shared_ptr<primitive::IPrimitive>> _primitives;
        std::vector<std::shared_ptr<light::ILight>> _lights;
        std::vector<std::shared_ptr<camera::ICamera>> _cameras;

        AmbientOcclusion _ambientOcclusion;
        AmbientLight _ambientLight;
        AmbientDiffuse _ambientDiffuse;

      private:
        void addPrimitive(const std::shared_ptr<IObject> &primitive);
        void addLight(const std::shared_ptr<IObject> &light);
        void addCamera(const std::shared_ptr<IObject> &camera);
        std::map<object::IObject::Type,
                 std::function<void(const std::shared_ptr<IObject> &)>>
            _addObjectHandlers = {
                {object::IObject::Type::PRIMITIVE,
                 [this](const std::shared_ptr<IObject> &obj) {
                     addPrimitive(obj);
                 }},
                {object::IObject::Type::LIGHT,
                 [this](const std::shared_ptr<IObject> &obj) {
                     addLight(obj);
                 }},
                {object::IObject::Type::CAMERA,
                 [this](const std::shared_ptr<IObject> &obj) {
                     addCamera(obj);
                 }},
            };
    };
}  // namespace raytracer::object::scene

#endif /* !ASCENE_HPP_ */
