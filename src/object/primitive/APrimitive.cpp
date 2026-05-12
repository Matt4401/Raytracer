/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** APrimitive
*/

#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    double APrimitive::hits(const maths::Ray &ray) const {
        if (HitRecord record; hits(ray, record)) {
            return record.t;
        }
        return -1.0;
    }

    const std::string &APrimitive::name() const noexcept {
        return _name;
    }

    maths::Vector APrimitive::center() const noexcept {
        return _center;
    }

    void APrimitive::setId(const int id) {
        _id = id;
    }

    int APrimitive::id() const {
        return _id;
    }

    void APrimitive::setLimitBox(const maths::AABoundingBox &box) {
        _sceneLimit = box;
        _hasLimit = true;
    }

    bool APrimitive::isInfinite() const {
        return false;
    }
}  // namespace raytracer::object::primitive
