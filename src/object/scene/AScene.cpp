/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AScene
*/

#include "object/AScene.hpp"

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

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

    void AScene::addPrimitive(object::AObject *primitive) {
        primitive::IPrimitive *primPtr =
            dynamic_cast<primitive::IPrimitive *>(primitive);
        if (!primPtr) {
            throw std::runtime_error("Invalid primitive object added to scene");
        }
        _primitives.push_back(std::unique_ptr<primitive::IPrimitive>(primPtr));
    }

    void AScene::addLight(object::AObject *light) {
        light::ILight *lightPtr = dynamic_cast<light::ILight *>(light);
        if (!lightPtr) {
            throw std::runtime_error("Failed to cast light object");
        }
        _lights.push_back(std::unique_ptr<light::ILight>(lightPtr));
    }

    void AScene::addCamera(object::AObject *camera) {
        camera::ICamera *camPtr = dynamic_cast<camera::ICamera *>(camera);
        if (!camPtr) {
            throw std::runtime_error("Invalid camera object added to scene");
        }
        _cameras.push_back(std::unique_ptr<camera::ICamera>(camPtr));
    }

    void AScene::addObject(IObject &object) {
        auto it = _addObjectHandlers.find(object.type());
        if (it != _addObjectHandlers.end()) {
            try {
                object::AObject &objRef =
                    dynamic_cast<object::AObject &>(object);
                it->second(&objRef);
            } catch (const std::bad_cast &) {
                throw std::runtime_error(
                    "Object type does not match expected type for handler");
            }
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