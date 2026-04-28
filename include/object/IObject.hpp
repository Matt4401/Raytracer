/*
** EPITECH PROJECT, 2026
** IObject
** File description:
** IObject header
*/

#pragma once

#include <any>
#include <functional>
#include <vector>

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

    typedef raytracer::object::IObject* (*BuilderFunc)(
        const std::vector<std::any>&);
  
    using BuilderFunc =
        raytracer::object::IObject *(*)(const std::vector<std::any> &);
    using buildFunction =
        std::function<IObject *(const std::vector<std::any> &param)>;

}  // namespace raytracer::object
