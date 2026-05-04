/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AScene
*/

#include "object/AScene.hpp"

#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::scene {
    AScene::AScene(const std::map<std::string, std::any> &params)
        : AObject(Type::SCENE) {
        _ambientOcclusion.samples =
            util::ObjectMiddleware::validate<int>(params, "aoSamples", "Scene");
        _ambientOcclusion.radius = util::ObjectMiddleware::validate<double>(
            params, "aoRadius", "Scene");
        util::Helpers::unsignedDouble(_ambientOcclusion.radius, "aoRadius",
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

    void AScene::addPrimitive(std::shared_ptr<object::IObject> primitive) {
        auto primitiveObj = std::dynamic_pointer_cast<primitive::IPrimitive>(primitive);
        if (!primitiveObj) {
            throw exception::PluginException("Failed to cast primitive object");
        }
        _primitives.push_back(primitiveObj);
    }

    void AScene::addLight(std::shared_ptr<object::IObject> light) {
        auto lightObj = std::dynamic_pointer_cast<light::ILight>(light);
        if (!lightObj) {
            throw exception::PluginException("Failed to cast light object");
        }
        _lights.push_back(lightObj);
    }

    void AScene::addCamera(std::shared_ptr<object::IObject> camera) {
        auto cameraObj = std::dynamic_pointer_cast<camera::ICamera>(camera);
        if (!cameraObj) {
            throw exception::PluginException("Failed to cast camera object");
        }
        _cameras.push_back(cameraObj);
    }

    void AScene::addObject(std::shared_ptr<object::IObject> object) {
        auto it = _addObjectHandlers.find(object->type());
        if (it != _addObjectHandlers.end()) {
            try {
                it->second(object);
            } catch (const std::bad_cast &) {
                throw exception::PluginException(
                    "Object type does not match expected type for handler");
            }
        } else {
            throw exception::PluginException(
                "Unsupported object type added to scene");
        }
    }

    void AScene::setAmbientLight(const maths::Color &color, double intensity) {
        _ambientLight = {color, intensity};
    }

    void AScene::setDiffuseLight(const maths::Color &color, double intensity) {
        _ambientDiffuse = {color, intensity};
    }

    void AScene::setAmbientOcclusion(int samples, double radius) {
        _ambientOcclusion = {samples, radius};
    }

    AmbientOcclusion AScene::ambientOcclusion() const {
        return _ambientOcclusion;
    }

    AmbientLight AScene::ambientLight() const {
        return _ambientLight;
    }

    AmbientDiffuse AScene::ambientDiffuse() const {
        return _ambientDiffuse;
    }

}  // namespace raytracer::object::scene