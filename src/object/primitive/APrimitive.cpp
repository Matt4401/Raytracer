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
}  // namespace raytracer::object::primitive
