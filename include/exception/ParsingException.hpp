/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ParsingException
*/

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class ParsingException : public Exception {
      public:
        template <typename... Args>
        explicit ParsingException(const std::string_view fmt,
                                  Args &&...args) noexcept
            : Exception(std::vformat(fmt, std::make_format_args(args...))) {
        }

        explicit ParsingException(const std::string_view msg) noexcept
            : Exception(std::string("[Parsing error]: ") + std::string(msg)) {
        }
    };
}  // namespace raytracer::exception
