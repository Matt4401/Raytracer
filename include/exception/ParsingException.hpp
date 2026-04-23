/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ParsingException
*/

#pragma once

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class ParsingException : public Exception {
      public:
        static constexpr auto PREFIX = "[Parsing error]: ";

        explicit ParsingException(
            const std::string &msg,
            const source &location = source::current()) noexcept
            : Exception(PREFIX + msg, location) {
        }
    };
}  // namespace raytracer::exception
