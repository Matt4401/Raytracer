/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include "plugin/PluginManager.hpp"
#include "plugin/ObjectFactory.hpp"

#include "math/Color.hpp"

#include "object/ALight.hpp"
#include "object/AScene.hpp"
#include "object/AObject.hpp"
#include "object/material/IMaterial.hpp"

#include <iostream>

namespace raytracer {

    void Core::init() {
        this->_plugManager.updatePluginList(PLUGINS_FOLDER_PATH);
        this->_plugManager.fillFactory(this->_objFactory);

        //EXMPLE SCENE WITHOUT CONFIGURATION FILE

        //SCENE
        std::map<std::string, std::any> sceneParams;
        sceneParams["aoSamples"] = 0; // Example: 16 samples for ambient occlusion 0 for no ambient occlusion
        sceneParams["aoRadius"] = 0.0; // Example: radius of 1.0 for ambient occlusion effect, 0 for no ambient occlusion
        sceneParams["ambientLightColor"] = maths::Color(0, 0, 0); // Example: white ambient light, 0 for no ambient light
        sceneParams["ambientLightIntensity"] = 0.0; // Example: low intensity for ambient light, 0 for no ambient light
        sceneParams["ambientDiffuseColor"] = maths::Color(0, 0, 0); // Example: white ambient diffuse light, 0 for no ambient diffuse light
        sceneParams["ambientDiffuseIntensity"] = 0.0; // Example: low intensity for ambient diffuse light, 0 for no ambient diffuse light

        auto scene = this->_objFactory.build("Scene", sceneParams);
       
        //PRIMITIVE
        // Flat material with red color
        std::map<std::string, std::any> redMaterialParams;
        redMaterialParams["color"] = maths::Color(0.75 * 255, 0.25 * 255, 0.25 * 255); // Red color
        redMaterialParams["reflType"] = raytracer::object::primitive::RefltT::DIFF; // Diffuse reflection
        redMaterialParams["emission"] = maths::Color(0, 0, 0); // No emission
        redMaterialParams["reflectivity"] = 0.0; // No reflection
        redMaterialParams["transparency"] = 0.0; // No transparency
        redMaterialParams["ior"] = 1.0; // Default index of refraction
        redMaterialParams["roughness"] = 0.0; // No roughness
        redMaterialParams["metalness"] = 0.0; // No metalness
        auto redMaterial = this->_objFactory.build("FlatColor", redMaterialParams);
        auto redMaterialPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(redMaterial);

        // Flat material with blue color

        std::map<std::string, std::any> blueMaterialParams;
        blueMaterialParams["color"] = maths::Color(0.25 * 255, 0.25 * 255, 0.75 * 255); // Blue color
        blueMaterialParams["reflType"] = raytracer::object::primitive::RefltT::DIFF; // Diffuse reflection
        blueMaterialParams["emission"] = maths::Color(0, 0, 0); // No emission
        blueMaterialParams["reflectivity"] = 0.0; // No reflection
        blueMaterialParams["transparency"] = 0.0; // No transparency
        blueMaterialParams["ior"] = 1.0; // Default index of refraction
        blueMaterialParams["roughness"] = 0.0; // No roughness
        blueMaterialParams["metalness"] = 0.0; // No metalness
        auto blueMaterial = this->_objFactory.build("FlatColor", blueMaterialParams);
        auto blueMaterialPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(blueMaterial);

        // Flat material with white color
        std::map<std::string, std::any> whiteMaterialParams;
        whiteMaterialParams["color"] = maths::Color(0.75 * 255, 0.75 * 255, 0.75 * 255); // White color
        whiteMaterialParams["reflType"] = raytracer::object::primitive::RefltT::DIFF; // Diffuse reflection
        whiteMaterialParams["emission"] = maths::Color(0, 0, 0); // No emission
        whiteMaterialParams["reflectivity"] = 0.0; // No reflection
        whiteMaterialParams["transparency"] = 0.0; // No transparency
        whiteMaterialParams["ior"] = 1.0; // Default index of refraction
        whiteMaterialParams["roughness"] = 0.0; // No roughness
        whiteMaterialParams["metalness"] = 0.0; // No metalness
        auto whiteMaterial = this->_objFactory.build("FlatColor", whiteMaterialParams);
        auto whiteMaterialPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(whiteMaterial);

        // Mirror material with white color
        std::map<std::string, std::any> mirrorMaterialParams;
        mirrorMaterialParams["color"] = maths::Color(0.999 * 255, 0.999 * 255, 0.999 * 255); // White color
        mirrorMaterialParams["reflType"] = raytracer::object::primitive::RefltT::SPEC; // Specular reflection
        mirrorMaterialParams["emission"] = maths::Color(0, 0, 0); // No emission
        mirrorMaterialParams["reflectivity"] = 1.0; // Full reflection
        mirrorMaterialParams["transparency"] = 0.0; // No transparency
        mirrorMaterialParams["ior"] = 1.0; // Default index of refraction
        mirrorMaterialParams["roughness"] = 0.0; // No roughness for perfect mirror
        mirrorMaterialParams["metalness"] = 1.0; // Full metalness for mirror
        auto mirrorMaterial = this->_objFactory.build("FlatColor", mirrorMaterialParams);
        auto mirrorMaterialPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(mirrorMaterial);

        // Basic Refractive material with white color
        std::map<std::string, std::any> refractiveMaterialParams;
        refractiveMaterialParams["color"] = maths::Color(0.999 * 255, 0.999 * 255, 0.999 * 255); // White color
        refractiveMaterialParams["reflType"] = raytracer::object::primitive::RefltT::REFR; // Refractive material
        refractiveMaterialParams["emission"] = maths::Color(0, 0, 0); // No emission
        refractiveMaterialParams["reflectivity"] = 0.0; // No reflection
        refractiveMaterialParams["transparency"] = 1.0; // Full transparency
        refractiveMaterialParams["ior"] = 1.5; // Example index of refraction for glass
        refractiveMaterialParams["roughness"] = 0.0; // No roughness for clear glass
        refractiveMaterialParams["metalness"] = 0.0; // No metalness for glass
        auto refractiveMaterial = this->_objFactory.build("FlatColor", refractiveMaterialParams);
        auto refractiveMaterialPtr = std::dynamic_pointer_cast<raytracer::object::material::IMaterial>(refractiveMaterial);


        // Red wall sphere
        std::map<std::string, std::any> redWallSphereParams;
        redWallSphereParams["center"] = maths::Vector(1e5 + 1, 40.8, 81.6); 
        redWallSphereParams["radius"] = 1e5;
        redWallSphereParams["material"] = redMaterialPtr;
        auto redWallSphere = this->_objFactory.build("Sphere", redWallSphereParams);

        // Blue wall sphere
        std::map<std::string, std::any> blueWallSphereParams;
        blueWallSphereParams["center"] = maths::Vector(-1e5 + 99, 40.8, 81.6); 
        blueWallSphereParams["radius"] = 1e5;
        blueWallSphereParams["material"] = blueMaterialPtr;
        auto blueWallSphere = this->_objFactory.build("Sphere", blueWallSphereParams);

        // Back white wall sphere
        std::map<std::string, std::any> backWallSphereParams;
        backWallSphereParams["center"] = maths::Vector(50, 40.8, 1e5); 
        backWallSphereParams["radius"] = 1e5;
        backWallSphereParams["material"] = whiteMaterialPtr;
        auto backWallSphere = this->_objFactory.build("Sphere", backWallSphereParams);

        // Mirror sphere
        std::map<std::string, std::any> mirrorSphereParams;
        mirrorSphereParams["center"] = maths::Vector(27, 16.5, 47); 
        mirrorSphereParams["radius"] = 16.5;
        mirrorSphereParams["material"] = mirrorMaterialPtr;
        auto mirrorSphere = this->_objFactory.build("Sphere", mirrorSphereParams);

        // Refractive sphere
        std::map<std::string, std::any> refractiveSphereParams;
        refractiveSphereParams["center"] = maths::Vector(73, 16.5, 78); 
        refractiveSphereParams["radius"] = 16.5;
        refractiveSphereParams["material"] = refractiveMaterialPtr;
        auto refractiveSphere = this->_objFactory.build("Sphere", refractiveSphereParams);

        // Point Light
        std::map<std::string, std::any> pointLightParams;
        pointLightParams["position"] = maths::Vector(50, 81.6 - 16.5, 81.6); // Position above the scene
        pointLightParams["color"] = maths::Color(255, 255, 255); // White light 
        pointLightParams["intensity"] = 1.0; // Full intensity
        auto pointLight = this->_objFactory.build("PointLight", pointLightParams);

        // Add objects to the scene
        auto scenePtr = std::dynamic_pointer_cast<raytracer::object::scene::IScene>(scene);
        if (scenePtr) {
            scenePtr->addObject(redWallSphere);
            scenePtr->addObject(blueWallSphere);
            scenePtr->addObject(backWallSphere);
            scenePtr->addObject(mirrorSphere);
            scenePtr->addObject(refractiveSphere);
            scenePtr->addObject(pointLight);
        } else {
            std::cerr << "Failed to cast scene to IScene interface.\n";
        }
        
    }

    void Core::run() {
        // TODO: main compute
    }

}  // namespace raytracer
