/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AScene
*/

#include "object/AScene.hpp"

#include <any>
#include <utility>

#include "bvh/BVHBuilder.hpp"
#include "exception/PluginException.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::scene {
    AScene::AScene(const std::map<std::string, std::any> &params)
        : AObject(Type::SCENE) {
        const auto &ambiantOcclusion = util::ObjectMiddleware::requireMap(
            params, "ambiantOcclusion", "Scene");
        const auto &ambientLight =
            util::ObjectMiddleware::requireMap(params, "ambientLight", "Scene");
        const auto &ambientDiffuse = util::ObjectMiddleware::requireMap(
            params, "ambientDiffuse", "Scene");

        this->_ambientOcclusion.samples = util::ObjectMiddleware::validate<int>(
            ambiantOcclusion, "samples", "Scene");
        this->_ambientOcclusion.radius =
            util::ObjectMiddleware::validate<double>(ambiantOcclusion, "radius",
                                                     "Scene");
        util::Helpers::unsignedDouble(_ambientOcclusion.radius, "radius",
                                      "Scene");

        this->_ambientLight.color =
            util::Helpers::toColor(ambientLight, "color", "Scene");
        this->_ambientLight.intensity =
            util::ObjectMiddleware::validate<double>(ambientLight, "intensity",
                                                     "Scene");
        util::Helpers::unsignedDouble(_ambientLight.intensity, "intensity",
                                      "Scene");

        _ambientDiffuse.ambient =
            util::Helpers::toColor(ambientDiffuse, "color", "Scene");
        _ambientDiffuse.intensity = util::ObjectMiddleware::validate<double>(
            ambientDiffuse, "intensity", "Scene");
        util::Helpers::unsignedDouble(_ambientDiffuse.intensity, "intensity",
                                      "Scene");

        _samplesPerPixel = util::ObjectMiddleware::optional<int>(
            params, "samplesPerPixel", 100, "Scene");
        util::Helpers::unsignedInt(_samplesPerPixel, "samplesPerPixel",
                                   "Scene");

        _bvhStrategy = util::ObjectMiddleware::optional<std::string>(
            params, "bvhStrategy", "", "Scene");
    }

    void AScene::addPrimitive(const std::shared_ptr<IObject> &primitive) {
        const auto primPtr =
            std::dynamic_pointer_cast<primitive::IPrimitive>(primitive);

        if (!primPtr) {
            throw exception::PluginException(
                "Invalid primitive object added to scene");
        }
        _primitives.push_back(primPtr);
    }

    void AScene::addLight(const std::shared_ptr<IObject> &light) {
        const auto lightPtr = std::dynamic_pointer_cast<light::ILight>(light);

        if (!lightPtr) {
            throw exception::PluginException("Failed to cast light object");
        }
        _lights.push_back(lightPtr);
    }

    void AScene::addCamera(const std::shared_ptr<IObject> &camera) {
        const auto camPtr = std::dynamic_pointer_cast<camera::ICamera>(camera);

        if (!camPtr) {
            throw exception::PluginException(
                "Invalid camera object added to scene");
        }
        _cameras.push_back(camPtr);
    }

    void AScene::addObject(const std::shared_ptr<IObject> object) {
        if (!object) {
            throw exception::PluginException("Null object added to scene");
        }
        if (const auto it = _addObjectHandlers.find(object->type());
            it != _addObjectHandlers.end()) {
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

    void AScene::setAmbientLight(const maths::Color &color,
                                 const double intensity) {
        _ambientLight = {color, intensity};
    }

    void AScene::setDiffuseLight(const maths::Color &color,
                                 const double intensity) {
        _ambientDiffuse = {color, intensity};
    }

    void AScene::setAmbientOcclusion(const int samples, const double radius) {
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

    bool AScene::haveCamera() {
        return this->_cameras.size() > 0;
    }

    int AScene::samplesPerPixel() const {
        return _samplesPerPixel;
    }

    std::string_view AScene::bvhStrategy() const {
        return _bvhStrategy;
    }

    void AScene::buildBVH(std::string_view strategy) {
        if (_primitives.empty())
            return;

        maths::AABoundingBox globalBox = _primitives[0]->boundingBox();
        bool first = true;

        for (const auto &prim : _primitives) {
            if (!prim->isInfinite()) {
                if (first) {
                    globalBox = prim->boundingBox();
                    first = false;
                } else {
                    globalBox.extend(prim->boundingBox());
                }
            }
        }
        for (const auto &cam : _cameras) globalBox.extend(cam->position());
        for (const auto &light : _lights) globalBox.extend(light->position());
        globalBox.pad(GLOBAL_BOX_PADDING);

        for (const auto &prim : _primitives) {
            prim->setLimitBox(globalBox);
        }
        for (std::size_t i = 0; i < _primitives.size(); ++i) {
            _primitives[i]->setId(static_cast<int>(i));
        }
        if (strategy.empty()) {
            _bvhRoot = nullptr;
            return;
        }
        auto builder =
            bvh::BVHBuilder<raytracer::bvh::ISplitStrategy>(strategy);
        _bvhRoot = builder.build(_primitives);
    }
}  // namespace raytracer::object::scene
