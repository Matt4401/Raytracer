/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** APrimitive
*/

#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    const std::string &APrimitive::name() const noexcept {
        return _name;
    }

    maths::Vector APrimitive::center() const noexcept {
        return _center;
    }

    maths::Vector APrimitive::emission() const noexcept {
        return _emission;
    }

    maths::Vector APrimitive::color() const noexcept {
        return _color;
    }

    RefltT APrimitive::refl() const noexcept {
        return _refl;
    }
}  // namespace raytracer::object::primitive
