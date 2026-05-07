/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Helpers
*/

#include "util/middleware/Helpers.hpp"

#include <any>
#include <map>
#include <string>
#include <string_view>

#include "exception/PluginException.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::util {

    void Helpers::unsignedInt(const int value, const std::string_view fieldName,
                              const std::string_view className) {
        if (value < 0) {
            throw exception::PluginException{"{} {} can't be negative: {}",
                                             className, fieldName, value};
        }
    }

    void Helpers::unsignedDouble(const double value,
                                 const std::string_view fieldName,
                                 const std::string_view className) {
        if (value < 0) {
            throw exception::PluginException{"{} {} can't be negative: {}",
                                             className, fieldName, value};
        }
    }

    object::primitive::RefltT Helpers::toEnumRefltT(
        const std::map<std::string, std::any> &params, std::string_view keyName,
        std::string_view className) {
        std::map<std::string, object::primitive::RefltT> refletMap = {
            {"DIFF", object::primitive::RefltT::DIFF},
            {"REFR", object::primitive::RefltT::REFR},
            {"SPEC", object::primitive::RefltT::SPEC},
        };
        std::string strReflect =
            ObjectMiddleware::validate<std::string>(params, keyName, className);

        auto iter = refletMap.find(strReflect);

        if (iter != refletMap.end()) {
            return iter->second;
        }
        std::string printableRefltTList = "/ ";

        for (const auto &reflt : refletMap)
            printableRefltTList += reflt.first + " / ";
        throw exception::PluginException{
            "given Reflection in {} has invalid value, "
            "must be in the following list: {}",
            std::string(className), printableRefltTList};
    }

    maths::Vector Helpers::normalVector(maths::Vector &normal) {
        if (normal == maths::Vector(0, 0, 0)) {
            throw exception::PluginException{
                "Normal vector cannot be the zero"};
        }
        normal.normalize();
        return normal;
    }

    void Helpers::notCollinearVector(const maths::Vector &v1,
                                     const maths::Vector &v2,
                                     std::string_view fieldName1,
                                     std::string_view fieldName2,
                                     const std::string_view className) {
        if (const double crossProductMagnitude = v1.cross(v2).magnitude();
            crossProductMagnitude == 0) {
            throw exception::PluginException{"{} {} and {} can't be collinear",
                                             className, fieldName1, fieldName2};
        }
    }

    maths::Color Helpers::toColor(const std::map<std::string, std::any> &params,
                                  const std::string_view keyName,
                                  const std::string_view className) {
        const auto &colorMap =
            ObjectMiddleware::requireMap(params, keyName, className);
        int rValue = ObjectMiddleware::validate<int>(colorMap, "r", className);
        int gValue = ObjectMiddleware::validate<int>(colorMap, "g", className);
        int bValue = ObjectMiddleware::validate<int>(colorMap, "b", className);

        if (rValue < 0 || rValue > 255 || gValue < 0 || gValue > 255 ||
            bValue < 0 || bValue > 255) {
            throw exception::PluginException{
                "color in {} has invalid value, must be between 0 and 255",
                className};
        }
        return maths::Color(rValue, gValue, bValue);
    }

    maths::Vector Helpers::toVector(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const std::string_view className) {
        const auto &vectorMap =
            ObjectMiddleware::requireMap(params, keyName, className);

        return maths::Vector(
            ObjectMiddleware::validate<double>(vectorMap, "x", className),
            ObjectMiddleware::validate<double>(vectorMap, "y", className),
            ObjectMiddleware::validate<double>(vectorMap, "z", className));
    }

    maths::Color Helpers::optionalColor(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const maths::Color &defaultValue,
        const std::string_view className) {
        if (!params.contains(std::string(keyName))) {
            return defaultValue;
        }
        return toColor(params, keyName, className);
    }

    maths::Vector Helpers::optionalVector(
        const std::map<std::string, std::any> &params,
        const std::string_view keyName, const maths::Vector &defaultValue,
        const std::string_view className) {
        if (!params.contains(std::string(keyName))) {
            return defaultValue;
        }
        return toVector(params, keyName, className);
    }
}  // namespace raytracer::util
