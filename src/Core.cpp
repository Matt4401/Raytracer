/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include "plugin/PluginManager.hpp"

#include "math/Color.hpp"

#include "object/ALight.hpp"
#include "object/AScene.hpp"
#include "object/AObject.hpp"

namespace raytracer {

    void Core::init() {
        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

        //EXMPLE SCNE WITHOUT CONFIGURATION FILE

        std::map<std::string, std::any> sceneParams;
        sceneParams["aoSamples"] = 16; // Example: 16 samples for ambient occlusion 0 for no ambient occlusion
        sceneParams["aoRadius"] = 1.0; // Example: radius of 1.0 for ambient occlusion effect, 0 for no ambient occlusion
        sceneParams["ambientLightColor"] = maths::Color(255, 255, 255); // Example: white ambient light, 0 for no ambient light
        sceneParams["ambientLightIntensity"] = 0.1; // Example: low intensity for ambient light, 0 for no ambient light
        sceneParams["ambientDiffuseColor"] = maths::Color(255, 255, 255); // Example: white ambient diffuse light, 0 for no ambient diffuse light
        sceneParams["ambientDiffuseIntensity"] = 0.2; // Example: low intensity for ambient diffuse light, 0 for no ambient diffuse light
        auto scene = std::make_shared<object::scene::IScene>(sceneParams);
    }

    void Core::run() {
        // TODO: main compute
    }

}  // namespace raytracer
