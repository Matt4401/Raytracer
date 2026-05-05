/*
** EPITECH PROJECT, 2026
** `
** File description:
** AObject
*/

#pragma once

#include "IObject.hpp"

namespace raytracer::object {
    class AObject : public IObject {
      public:
        explicit AObject(Type type) : _type(type) {};
        ~AObject() override = default;
        Type type() const override {
            return _type;
        };

      protected:
        static constexpr double K_RAY_EPSILON = 1e-4;

        Type _type;
    };
}  // namespace raytracer::object
