/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AScene
*/

#include "AScene.hpp"

#include "util/Helpers.hpp"
#include "util/ObjectMiddleware.hpp"

namespace raytracer::object::scene {
    AScene::AScene(const std::map<std::string, std::any> &params)
        : AObject(Type::SCENE) {
        _ambiantOcclusion.samples =
            util::ObjectMiddleware::validate<int>(params, "aoSamples", "Scene");
        _ambiantOcclusion.radius = util::ObjectMiddleware::validate<double>(
            params, "aoRadius", "Scene");
        util::Helpers::unsignedDouble(_ambiantOcclusion.radius, "aoRadius",
                                      "Scene");
        _ambientLight.color =
            util::Helpers::toColor(params, "ambientLightColor", "Scene");
        _ambientLight.intensity = util::ObjectMiddleware::validate<double>(
            params, "ambientLightIntensity", "Scene");
        util::Helpers::unsignedDouble(_ambientLight.intensity,
                                      "ambientLightIntensity", "Scene");
        _ambientDiffuse.ambient =
            util::Helpers::toColor(params, "ambientDiffuseColor", "Scene");
        _ambientDiffuse.intensity = util::ObjectMiddleware::validate<double>(
            params, "ambientDiffuseIntensity", "Scene");
        util::Helpers::unsignedDouble(_ambientDiffuse.intensity,
                                      "ambientDiffuseIntensity", "Scene");
    }

    void AScene::addPrimitive(const object::IObject &primitive) {
        primitive::IPrimitive *primitive =
            dynamic_cast<const primitive::IPrimitive *>(&primitive);
        if (!primitive) {
            throw std::runtime_error("Invalid primitive object added to scene");
        }
        _primitives.push_back(
            std::unique_ptr<primitive::IPrimitive>(primitive));
    }

    void AScene::addLight(const object::IObject &light) {
        light::ILight *light = dynamic_cast<const light::ILight *>(&light);
        if (!light) {
            throw std::runtime_error("Invalid light object added to scene");
        }
        _lights.push_back(std::unique_ptr<light::ILight>(light));
    }

    void AScene::addCamera(const object::IObject &camera) {
        camera::ICamera *cam = dynamic_cast<const camera::ICamera *>(&camera);
        if (!cam) {
            throw std::runtime_error("Invalid camera object added to scene");
        }
        _cameras.push_back(std::unique_ptr<camera::ICamera>(cam));
    }

    void AScene::addObject(const IObject &object) {
        auto it = _addObjectHandlers.find(object.getType());
        if (it != _addObjectHandlers.end()) {
            it->second(object);
        } else {
            throw std::runtime_error("Unsupported object type added to scene");
        }
    }

    void AScene::setAmbientLight(const maths::Color &color, double intensity) {
        _ambientLight = {color, intensity};
    }

    void AScene::setDiffuseLight(const maths::Color &color, double intensity) {
        _ambientDiffuse = {color, intensity};
    }

    void AScene::setAmbientOcclusion(int samples, double radius) {
        _ambiantOcclusion = {samples, radius};
    }

    AmbiantOcclusion AScene::ambiantOcclusion() const {
        return _ambiantOcclusion;
    }

    AmbientLight AScene::ambientLight() const {
        return _ambientLight;
    }

    AmbientDiffuse AScene::ambientDiffuse() const {
        return _ambientDiffuse;
    }

}  // namespace raytracer::object::scene