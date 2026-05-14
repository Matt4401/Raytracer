/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Translation
*/

#pragma once

#include <any>
#include <map>
#include "object/transformations/ATransformDecorator.hpp"

namespace raytracer::object::primitive {
    class Translation : public ATransformDecorator {
      public:
        explicit Translation(const std::map<std::string, std::any> &params);
        ~Translation() override = default;
    };
}  // namespace raytracer::object::primitive
