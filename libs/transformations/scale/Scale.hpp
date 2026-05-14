/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Scale
*/

#pragma once

#include <any>
#include <map>
#include "object/transformations/ATransformDecorator.hpp"

namespace raytracer::object::primitive {
    class Scale : public ATransformDecorator {
      public:
        explicit Scale(const std::map<std::string, std::any> &params);
        ~Scale() override = default;
    };
}  // namespace raytracer::object::primitive
