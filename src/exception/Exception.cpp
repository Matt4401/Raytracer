/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exception
*/

#pragma once

#include "exception/Exception.hpp"

#include <sstream>
#include <string>

namespace raytracer::exception {
    Exception::Exception(const std::string &message,
                         const source &location) noexcept
        : _message(message),
          _location(location),
          _fullMessage(buildFullMessage()) {
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
        oss << _message << " at " << getLocationString();
        return oss.str();
    }

    std::string Exception::getLocationString() const noexcept {
        std::ostringstream oss;
        oss << _location.file_name() << ":" << _location.line();
        return oss.str();
    }
}  // namespace raytracer::exception
