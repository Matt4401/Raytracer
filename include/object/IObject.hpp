/*
** EPITECH PROJECT, 2026
** IObject
** File description:
** IObject header
*/

#pragma once

#include <any>
#include <functional>
#include <memory>

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

    using buildFunction =
        std::function<std::unique_ptr<IObject>(std::vector<std::any>)>;

}  // namespace raytracer::object
