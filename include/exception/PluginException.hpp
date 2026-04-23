/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginException
*/

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class PluginException : public Exception {
      public:
        template <typename... Args>
        explicit PluginException(const std::string_view fmt,
                                 Args &&...args) noexcept
            : Exception(std::vformat(fmt, std::make_format_args(args...))) {
        }

        explicit PluginException(const std::string_view msg) noexcept
            : Exception(std::string("[Plugin Error]: ") + std::string(msg)) {
        }
    };
}  // namespace raytracer::exception
