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
#include <utility>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class ParsingException : public Exception {
      public:
        static constexpr std::string_view PREFIX = "[Parsing error]: ";

        template <typename... Args>
        explicit ParsingException(const std::string_view fmt, Args &&...args)
            : Exception(std::string(PREFIX) +
                            std::vformat(fmt, std::make_format_args(args...)),
                        source::current()) {
        }

        explicit ParsingException(const std::string_view msg,
                                  const source &location = source::current())
            : Exception(std::string(PREFIX) + std::string(msg), location) {
        }
    };
}  // namespace raytracer::exception
