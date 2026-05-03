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

    void AScene::addPrimitive(
        const std::shared_ptr<object::AObject> &primitive) {
        auto primPtr =
            std::dynamic_pointer_cast<primitive::IPrimitive>(primitive);
        if (!primPtr) {
            throw exception::PluginException(
                "Invalid primitive object added to scene");
        }
        _primitives.push_back(primPtr);
    }

    void AScene::addLight(const std::shared_ptr<object::AObject> &light) {
        auto lightPtr = std::dynamic_pointer_cast<light::ILight>(light);
        if (!lightPtr) {
            throw exception::PluginException("Failed to cast light object");
        }
        _lights.push_back(lightPtr);
    }

    void AScene::addCamera(const std::shared_ptr<object::AObject> &camera) {
        auto camPtr = std::dynamic_pointer_cast<camera::ICamera>(camera);
        if (!camPtr) {
            throw exception::PluginException(
                "Invalid camera object added to scene");
        }
        _cameras.push_back(camPtr);
    }

    void AScene::addObject(std::shared_ptr<IObject> object) {
        if (!object) {
            throw exception::PluginException("Null object added to scene");
        }
        auto it = _addObjectHandlers.find(object->type());
        if (it != _addObjectHandlers.end()) {
            try {
                auto objRef =
                    std::dynamic_pointer_cast<object::AObject>(object);
                if (!objRef) {
                    throw std::bad_cast();
                }
                it->second(objRef);
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

    const std::vector<std::shared_ptr<primitive::IPrimitive>> &
    AScene::primitives() const {
        return _primitives;
    }

    const std::vector<std::shared_ptr<light::ILight>> &AScene::lights() const {
        return _lights;
    }

    const std::vector<std::shared_ptr<camera::ICamera>> &AScene::cameras()
        const {
        return _cameras;
    }

}  // namespace raytracer::object::scene