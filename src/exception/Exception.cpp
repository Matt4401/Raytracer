/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exception
*/

#pragma once

#include "exception/Exception.hpp"

#include <source_location>
#include <sstream>
#include <string>
#include <string_view>

namespace raytracer::exception {
    Exception::Exception(const std::string_view message,
                         const source &location) noexcept
        : _message(message), _location(location) {
        _fullMessage = buildFullMessage();
    }

    const char *Exception::what() const noexcept {
        return _fullMessage.c_str();
    }

    const source &Exception::location() const noexcept {
        return _location;
    }

    std::string Exception::buildFullMessage() const noexcept {
        std::ostringstream oss;
        oss << _message << " in " << _location.file_name() << " at line "
            << _location.line();
        return oss.str();
    }
}  // namespace raytracer::exception
