/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exception
*/

#pragma once

#include <exception>
#include <source_location>
#include <string>

namespace raytracer::exception {
    using source = std::source_location;

    class Exception : public std::exception {
      public:
        explicit Exception(const std::string &message,
                           const source &location = source::current()) noexcept;
        Exception(const Exception &) = default;
        Exception(Exception &&) = default;
        ~Exception() override = default;

        Exception &operator=(const Exception &) = default;
        Exception &operator=(Exception &&) = default;

        const char *what() const noexcept override;
        const source &location() const noexcept;

      protected:
        std::string _message;
        std::source_location _location;
        std::string _fullMessage;

        [[nodiscard]] std::string buildFullMessage() const noexcept;
        [[nodiscard]] std::string getLocationString() const noexcept;
    };
}  // namespace raytracer::exception
