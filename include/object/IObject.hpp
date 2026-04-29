/*
** EPITECH PROJECT, 2026
** IObject
** File description:
** IObject header
*/

#pragma once

#include <any>
#include <functional>
#include <map>
#include <string>

namespace raytracer::object {
    class IObject {
      public:
        enum class Type {
            PRIMITIVE,
            LIGHT,
            MATERIAL,
            SCENE,
            CAMERA,
            TRANSFORM
        };

        virtual ~IObject() = default;
        virtual Type type() const = 0;
    };

    using BuilderFunc = raytracer::object::IObject
        *(*)(const std::map<std::string, std::any> &);
    using buildFunction =
        std::function<IObject *(const std::map<std::string, std::any> &param)>;

}  // namespace raytracer::object
