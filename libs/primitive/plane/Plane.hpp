/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
*/

#pragma once

#include <any>
#include <vector>

namespace raytracer::object::primitive {
    class Plane {
      public:
        explicit Plane(const std::vector<std::any>& args);
        ~Plane() = default;

      private:
        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}  // namespace raytracer::object::primitive
