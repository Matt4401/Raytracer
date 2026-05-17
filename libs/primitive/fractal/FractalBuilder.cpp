/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** FractalBuilder
*/

#include "fractal/FractalBuilder.hpp"

#include <any>
#include <map>
#include <memory>
#include <string>

#include "exception/PluginException.hpp"
#include "fractal/Fractal3D.hpp"
#include "fractal/MandelbulbStrategy.hpp"
#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {
    std::shared_ptr<IFractalStrategy> buildMandelbulbStrategy(
        const std::map<std::string, std::any> &params,
        const std::string &contextName) {
        const double power = util::Helpers::readNumeric(params, "power", 8.0);
        const int iterations =
            util::Helpers::readInt(params, "iterations", 100);
        const double bailout =
            util::Helpers::readNumeric(params, "bailout", 2.0);

        util::Helpers::unsignedDouble(power, "power", contextName);
        util::Helpers::unsignedInt(iterations, "iterations", contextName);
        util::Helpers::unsignedDouble(bailout, "bailout", contextName);
        return std::make_shared<MandelbulbStrategy>(power, iterations, bailout);
    }

    IObject *buildMandelbulb(const std::map<std::string, std::any> &params,
                             const std::string &contextName) {
        const std::string fractalName =
            util::ObjectMiddleware::validate<std::string>(params, "name",
                                                          contextName);

        if (fractalName != "mandelbulb") {
            throw raytracer::exception::PluginException(
                "Unsupported fractal '{}'", fractalName);
        }

        const auto strategy = buildMandelbulbStrategy(params, contextName);
        const auto center =
            util::Helpers::toVector(params, "center", contextName);
        const int maxSteps = util::Helpers::readInt(params, "maxSteps", 200);
        const double epsilon =
            util::Helpers::readNumeric(params, "epsilon", 1e-5);
        const double maxDist =
            util::Helpers::readNumeric(params, "maxDist", 100.0);
        const auto material = util::ObjectMiddleware::optional(
            params, "material",
            std::shared_ptr<raytracer::object::material::IMaterial>{},
            contextName);

        util::Helpers::unsignedInt(maxSteps, "maxSteps", contextName);
        util::Helpers::unsignedDouble(epsilon, "epsilon", contextName);
        util::Helpers::unsignedDouble(maxDist, "maxDist", contextName);
        if (material) {
            return new Fractal3D(material, strategy, center, maxSteps, epsilon,
                                 maxDist);
        }
        return new Fractal3D(strategy, center, maxSteps, epsilon, maxDist);
    }
}  // namespace raytracer::object::primitive