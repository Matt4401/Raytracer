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
#include <map>
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

        bool intersect(const maths::Ray &ray, double &t,
                       int &objectId) const override = 0;
        maths::Vector radiance(const maths::Ray &ray, int depth,
                               unsigned short *xi,
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

        void buildBVH(std::string_view strategy) override;

      protected:
        static constexpr int K_MAX_RADIANCE_DEPTH = 10;
        static constexpr int K_DIFFUSE_RUSSIAN_ROULETTE_DEPTH = 5;
        static constexpr int K_REFRACTIVE_RUSSIAN_ROULETTE_DEPTH = 2;
        static constexpr double K_DEFAULT_IOR = 1.5;
        static constexpr double K_DIELECTRIC_F0 = 0.04;
        static constexpr double K_PROB_NORMALIZATION_THRESHOLD = 1e-12;

        std::vector<std::shared_ptr<primitive::IPrimitive>> _primitives;
        std::shared_ptr<primitive::IPrimitive> _bvhRoot;
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
