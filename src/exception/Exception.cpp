/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exception
*/

#include "exception/Exception.hpp"

#include <source_location>
#include <sstream>
#include <string>
#include <string_view>

namespace raytracer::exception {
    /**
     * @brief Builds an exception with a message and captured source location.
     * @param message Human-readable error description.
     * @param location Call site metadata, defaults to std::source_location::current().
     */
    Exception::Exception(const std::string_view message,
                         const source &location)
        : _message(message), _location(location) {
        _fullMessage = buildFullMessage();
    }

    /**
     * @brief Returns the fully formatted exception message.
     * @return Null-terminated message buffer owned by this exception instance.
     */
    const char *Exception::what() const noexcept {
        return _fullMessage.c_str();
    }

    /**
     * @brief Returns the source location associated with this exception.
     * @return Reference to the stored std::source_location.
     */
    const source &Exception::location() const noexcept {
        return _location;
    }

    /**
     * @brief Composes a full message including file and line information.
     * @return Formatted string: "<message> in <file> at line <line>".
     */
    std::string Exception::buildFullMessage() const {
        std::ostringstream oss;
        oss << _message << " in " << _location.file_name() << " at line "
            << _location.line();
        return oss.str();
    }
}  // namespace raytracer::exception
