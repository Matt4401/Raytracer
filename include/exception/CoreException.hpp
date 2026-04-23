/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CoreException
*/

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class CoreException : public Exception {
      public:
        template <typename... Args>
        explicit CoreException(const std::string_view fmt,
                               Args &&...args) noexcept
            : Exception(std::vformat(fmt, std::make_format_args(args...))) {
        }

        explicit CoreException(const std::string_view msg) noexcept
            : Exception(std::string("[Core Error]: ") + std::string(msg)) {
        }
    };
}  // namespace raytracer::exception
