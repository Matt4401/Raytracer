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

namespace object {

    class IObject {
      public:
        virtual ~IObject() = default;
    };

    using buildFunction =
        std::function<std::unique_ptr<IObject>(std::vector<std::any>)>;

}  // namespace object
