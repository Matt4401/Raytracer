/*
** EPITECH PROJECT, 2026
** IObject
** File description:
** IObject header
*/

#pragma once

#include <any>
#include <functional>

namespace object {

    class IObject {
      public:
        virtual ~IObject() = default;
    };

    using buildFunction =
        std::function<IObject *(const std::vector<std::any> &param)>;

}  // namespace object
