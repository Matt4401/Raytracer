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
#include <utility>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class CoreException : public Exception {
      public:
        static constexpr std::string_view PREFIX = "[Core error]: ";

        template <typename... Args>
        explicit CoreException(const std::string_view fmt, Args &&...args)
            : Exception(std::string(PREFIX) +
                            std::vformat(fmt, std::make_format_args(args...)),
                        source::current()) {
        }

        explicit CoreException(const std::string_view msg,
                               const source &location = source::current())
            : Exception(std::string(PREFIX) + std::string(msg), location) {
        }
    };
}  // namespace raytracer::exception
