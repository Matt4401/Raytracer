/*
** EPITECH PROJECT, 2026
** Test
** File description:
** Test header
*/

#pragma once

#include "object/IObject.hpp"

namespace raytracer::tests {

    class ValidObject : public object::IObject {
      public:
        ValidObject() = default;
        ~ValidObject() override = default;

        Type type() const override {
            return Type::PRIMITIVE;
        }
    };
}  // namespace raytracer::tests
