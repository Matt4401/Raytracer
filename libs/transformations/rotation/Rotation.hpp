/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Rotation
*/

#pragma once

#include <any>
#include <map>
#include "object/transformations/ATransformDecorator.hpp"

namespace raytracer::object::primitive {
    class Rotation : public ATransformDecorator {
      public:
        explicit Rotation(const std::map<std::string, std::any> &params);
        ~Rotation() override = default;
    };
}  // namespace raytracer::object::primitive
