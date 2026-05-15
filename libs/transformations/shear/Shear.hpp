/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Shear
*/

#pragma once

#include <any>
#include <map>

#include "object/transformations/ATransformDecorator.hpp"

namespace raytracer::object::primitive {
    class Shear : public ATransformDecorator {
      public:
        explicit Shear(const std::map<std::string, std::any> &params);
        ~Shear() override = default;
    };
}  // namespace raytracer::object::primitive
