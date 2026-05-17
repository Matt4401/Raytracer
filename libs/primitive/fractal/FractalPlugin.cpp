/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** FractalPlugin
*/

#include <any>
#include <map>
#include <object/AObject.hpp>
#include <string>

#include "exception/PluginException.hpp"
#include "fractal/FractalBuilder.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

extern "C" raytracer::object::AObject::Type type(void) {
    return raytracer::object::AObject::Type::PRIMITIVE;
}

extern "C" const char *name(void) {
    return "fractal";
}

extern "C" raytracer::object::BuilderFunc builder(void) {
    return [](const std::map<std::string, std::any> &params)
               -> raytracer::object::IObject * {
        const std::string fractalName =
            raytracer::util::ObjectMiddleware::validate<std::string>(
                params, "name", "FractalPlugin");

        if (fractalName == "mandelbulb") {
            return raytracer::object::primitive::buildMandelbulb(
                params, "FractalPlugin");
        }
        throw raytracer::exception::PluginException("Unsupported fractal '{}'",
                                                    fractalName);
    };
}
