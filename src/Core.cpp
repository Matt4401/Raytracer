/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <iostream>

#include "object/AObject.hpp"
#include "object/material/IMaterial.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init() {
        auto vec = [](double x, double y, double z) {
            return std::map<std::string, std::any>{
                {"x", x},
                {"y", y},
                {"z", z},
            };
        };
        auto color = [](unsigned char r, unsigned char g, unsigned char b) {
            return std::map<std::string, std::any>{
                {"r", r},
                {"g", g},
                {"b", b},
            };
        };

        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

        // EXMPLE SCENE WITHOUT CONFIGURATION FILE

        // SCENE
        std::map<std::string, std::any> sceneParams;
        sceneParams["aoSamples"] = 0;  // Example: 16 samples for ambient
                                       // occlusion 0 for no ambient occlusion
        sceneParams["aoRadius"] =
            0.0;  // Example: radius of 1.0 for ambient occlusion effect, 0
                  // for no ambient occlusion
        sceneParams["ambientLightColor"] = color(
            0, 0, 0);  // Example: white ambient light, 0 for no ambient light
        sceneParams["ambientLightIntensity"] =
            0.0;  // Example: low intensity for ambient light, 0
                  // for no ambient light
        sceneParams["ambientDiffuseColor"] =
            color(0, 0, 0);  // Example: white ambient diffuse light, 0
                             // for no ambient diffuse light
        sceneParams["ambientDiffuseIntensity"] =
            0.0;  // Example: low intensity for ambient diffuse
                  // light, 0 for no ambient diffuse light
        auto scene = this->_objFactory.build("scene", sceneParams);

        // CAMERA
        std::map<std::string, std::any> cameraParams;
        cameraParams["position"] =
            vec(50.0, 52.0, 295.6);  // Example: camera positioned at (0, 0, 5)
        cameraParams["rotation"] = vec(
            0.0, -0.042612, -1.0);  // Example: no rotation (looking towards -Z)
        cameraParams["fieldOfView"] =
            90.0;  // Example: 90 degree field of view, adjust as needed
        cameraParams["aspectRatio"] =
            16.0 / 9.0;  // Example: standard widescreen aspect ratio
        cameraParams["resolution"] = std::map<std::string, std::any>{
            {"width", 400}, {"height", 200}};  // Example: 400x200 resolution
        auto camera = this->_objFactory.build("camera", cameraParams);

        // PRIMITIVE
        //  Flat material with red color
        std::map<std::string, std::any> redMaterialParams;
        redMaterialParams["color"] = color(191, 63, 63);  // Red color
        redMaterialParams["reflType"] =
            raytracer::object::primitive::RefltT::DIFF;  // Diffuse reflection
        redMaterialParams["emission"] = vec(0.0, 0.0, 0.0);  // No emission
        redMaterialParams["reflectivity"] = 0.0;             // No reflection
        redMaterialParams["transparency"] = 0.0;             // No transparency
        redMaterialParams["ior"] = 1.0;        // Default index of refraction
        redMaterialParams["roughness"] = 0.0;  // No roughness
        redMaterialParams["metalness"] = 0.0;  // No metalness
        auto redMaterial =
            this->_objFactory.build("flatcolor", redMaterialParams);
        auto redMaterialPtr =
            std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
                redMaterial);

        // Flat material with blue color

        std::map<std::string, std::any> blueMaterialParams;
        blueMaterialParams["color"] = color(63, 63, 191);  // Blue color
        blueMaterialParams["reflType"] =
            raytracer::object::primitive::RefltT::DIFF;  // Diffuse reflection
        blueMaterialParams["emission"] = vec(0.0, 0.0, 0.0);  // No emission
        blueMaterialParams["reflectivity"] = 0.0;             // No reflection
        blueMaterialParams["transparency"] = 0.0;             // No transparency
        blueMaterialParams["ior"] = 1.0;        // Default index of refraction
        blueMaterialParams["roughness"] = 0.0;  // No roughness
        blueMaterialParams["metalness"] = 0.0;  // No metalness
        auto blueMaterial =
            this->_objFactory.build("flatcolor", blueMaterialParams);
        auto blueMaterialPtr =
            std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
                blueMaterial);

        // Flat material with white color
        std::map<std::string, std::any> whiteMaterialParams;
        whiteMaterialParams["color"] = color(191, 191, 191);  // White color
        whiteMaterialParams["reflType"] =
            raytracer::object::primitive::RefltT::DIFF;  // Diffuse reflection
        whiteMaterialParams["emission"] = vec(0.0, 0.0, 0.0);  // No emission
        whiteMaterialParams["reflectivity"] = 0.0;             // No reflection
        whiteMaterialParams["transparency"] = 0.0;  // No transparency
        whiteMaterialParams["ior"] = 1.0;        // Default index of refraction
        whiteMaterialParams["roughness"] = 0.0;  // No roughness
        whiteMaterialParams["metalness"] = 0.0;  // No metalness
        auto whiteMaterial =
            this->_objFactory.build("flatcolor", whiteMaterialParams);
        auto whiteMaterialPtr =
            std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
                whiteMaterial);

        // Mirror material with white color
        std::map<std::string, std::any> mirrorMaterialParams;
        mirrorMaterialParams["color"] = color(254, 254, 254);  // White color
        mirrorMaterialParams["reflType"] =
            raytracer::object::primitive::RefltT::SPEC;  // Specular reflection
        mirrorMaterialParams["emission"] = vec(0.0, 0.0, 0.0);  // No emission
        mirrorMaterialParams["reflectivity"] = 1.0;  // Full reflection
        mirrorMaterialParams["transparency"] = 0.0;  // Opaque mirror
        mirrorMaterialParams["ior"] = 1.0;  // Default index of refraction
        mirrorMaterialParams["roughness"] =
            0.0;  // No roughness for perfect mirror
        mirrorMaterialParams["metalness"] = 1.0;  // Full metalness for mirror
        auto mirrorMaterial =
            this->_objFactory.build("flatcolor", mirrorMaterialParams);
        auto mirrorMaterialPtr =
            std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
                mirrorMaterial);

        // Basic Refractive material with white color
        std::map<std::string, std::any> refractiveMaterialParams;
        refractiveMaterialParams["color"] =
            color(254, 254, 254);  // White color
        refractiveMaterialParams["reflType"] =
            raytracer::object::primitive::RefltT::REFR;  // Refractive material
        refractiveMaterialParams["emission"] =
            vec(0.0, 0.0, 0.0);  // No emission
        // For physical glass, let Fresnel (IOR) drive reflectance.
        refractiveMaterialParams["reflectivity"] =
            0.0;  // don't force reflection
        refractiveMaterialParams["transparency"] = 1.0;  // allow transmission
        refractiveMaterialParams["ior"] =
            1.5;  // Example index of refraction for glass
        refractiveMaterialParams["roughness"] =
            0.0;  // No roughness for clear glass
        refractiveMaterialParams["metalness"] = 0.0;  // No metalness for glass
        auto refractiveMaterial =
            this->_objFactory.build("flatcolor", refractiveMaterialParams);
        auto refractiveMaterialPtr =
            std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(
                refractiveMaterial);

        // Mur Gauche (Rouge), plan x = 1
        std::map<std::string, std::any> leftWallParams;
        leftWallParams["center"] = vec(1.0, 0.0, 0.0);
        leftWallParams["maxPoint"] = vec(1.0, 81.6, 170.0);
        leftWallParams["normal"] = vec(1.0, 0.0, 0.0);
        leftWallParams["material"] = redMaterialPtr;
        auto leftWall = this->_objFactory.build("rectangle", leftWallParams);

        // Mur Droit (Bleu), plan x = 99
        std::map<std::string, std::any> rightWallParams;
        rightWallParams["center"] = vec(99.0, 0.0, 0.0);
        rightWallParams["maxPoint"] = vec(99.0, 81.6, 170.0);
        rightWallParams["normal"] = vec(-1.0, 0.0, 0.0);
        rightWallParams["material"] = blueMaterialPtr;
        auto rightWall = this->_objFactory.build("rectangle", rightWallParams);

        // Mur du Fond (Blanc), plan z = 0
        std::map<std::string, std::any> backWallParams;
        backWallParams["center"] = vec(1.0, 0.0, 0.0);
        backWallParams["maxPoint"] = vec(99.0, 81.6, 0.0);
        backWallParams["normal"] = vec(0.0, 0.0, 1.0);
        backWallParams["material"] = whiteMaterialPtr;
        auto backWall = this->_objFactory.build("rectangle", backWallParams);

        // Sol (Blanc), plan y = 0
        std::map<std::string, std::any> floorParams;
        floorParams["center"] = vec(1.0, 0.0, 0.0);
        floorParams["maxPoint"] = vec(99.0, 0.0, 170.0);
        floorParams["normal"] = vec(0.0, 1.0, 0.0);
        floorParams["material"] = whiteMaterialPtr;
        auto floor = this->_objFactory.build("rectangle", floorParams);

        // Plafond (Blanc), plan y = 81.6
        std::map<std::string, std::any> ceilingParams;
        ceilingParams["center"] = vec(1.0, 81.6, 0.0);
        ceilingParams["maxPoint"] = vec(99.0, 81.6, 170.0);
        ceilingParams["normal"] = vec(0.0, -1.0, 0.0);
        ceilingParams["material"] = whiteMaterialPtr;
        auto ceiling = this->_objFactory.build("rectangle", ceilingParams);

        // Mirror sphere
        std::map<std::string, std::any> mirrorSphereParams;
        mirrorSphereParams["center"] = vec(27.0, 16.5, 47.0);
        mirrorSphereParams["radius"] = 16.5;
        mirrorSphereParams["material"] = mirrorMaterialPtr;
        auto mirrorSphere =
            this->_objFactory.build("sphere", mirrorSphereParams);

        // Refractive sphere
        std::map<std::string, std::any> refractiveSphereParams;
        refractiveSphereParams["center"] = vec(73.0, 16.5, 78.0);
        refractiveSphereParams["radius"] = 16.5;
        refractiveSphereParams["material"] = refractiveMaterialPtr;
        auto refractiveSphere =
            this->_objFactory.build("sphere", refractiveSphereParams);

        // Point Light
        std::map<std::string, std::any> pointLightParams;
        pointLightParams["position"] =
            vec(50.0, 81.6 - 24.5, 81.6);  // Position above the scene
        pointLightParams["color"] = color(250, 230, 230);  // White light
        pointLightParams["intensity"] = 400.0;
        pointLightParams["radius"] = 2.0;
        auto pointLight =
            this->_objFactory.build("point_light", pointLightParams);

        // Add objects to the scene
        auto sceneObj =
            std::dynamic_pointer_cast<raytracer::object::AObject>(scene);
        auto scenePtr =
            std::dynamic_pointer_cast<raytracer::object::scene::IScene>(
                sceneObj);
        if (scenePtr) {
            scenePtr->addObject(leftWall);
            scenePtr->addObject(rightWall);
            scenePtr->addObject(backWall);
            scenePtr->addObject(floor);
            scenePtr->addObject(ceiling);
            scenePtr->addObject(mirrorSphere);
            scenePtr->addObject(refractiveSphere);
            scenePtr->addObject(pointLight);
            scenePtr->addObject(camera);
        } else {
            std::cerr << "Failed to create scene object\n";
        }
        _scene = scenePtr;
        _scene->buildBVH("sah");  // to change
    }

    void Core::run() {
        this->_renderer.render(*_scene, 1, 100);
        this->_renderer.pixelToPPM(*_scene);
    }

}  // namespace raytracer
