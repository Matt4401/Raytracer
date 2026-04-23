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
        explicit PluginException(const std::string_view prefix,
                                 const std::string_view fmt,
                                 Args &&...args) noexcept
            : Exception(std::string(prefix) +
                        std::vformat(fmt, std::make_format_args(args...))) {
        }

        template <typename... Args>
        explicit PluginException(std::string_view fmt, Args &&...args) noexcept
            : PluginException("[Plugin error]: ", fmt,
                              std::forward<Args>(args)...) {
        }

        explicit PluginException(const std::string_view msg) noexcept
            : Exception(std::string("[Plugin error]: ") + std::string(msg)) {
        }
    };

    class PrimitivesException : public PluginException {
      public:
        template <typename... Args>
        explicit PrimitivesException(std::string_view fmt,
                                     Args &&...args) noexcept
            : PluginException("[Plugin > Primitive]: ", fmt,
                              std::forward<Args>(args)...) {
        }

        explicit PrimitivesException(const std::string_view msg) noexcept
            : PluginException("[Plugin > Primitive]: ", msg) {
        }
    };

    class TransformationException : public PluginException {
      public:
        template <typename... Args>
        explicit TransformationException(std::string_view fmt,
                                         Args &&...args) noexcept
            : PluginException("[Plugin > Transformation]: ", fmt,
                              std::forward<Args>(args)...) {
        }

        explicit TransformationException(const std::string_view msg) noexcept
            : PluginException("[Plugin > Transformation]: ", msg) {
        }
    };

    class LightException : public PluginException {
      public:
        template <typename... Args>
        explicit LightException(std::string_view fmt, Args &&...args) noexcept
            : PluginException("[Plugin > Light]: ", fmt,
                              std::forward<Args>(args)...) {
        }

        explicit LightException(const std::string_view msg) noexcept
            : PluginException("[Plugin > Light]: ", msg) {
        }
    };

    class MaterialException : public PluginException {
      public:
        template <typename... Args>
        explicit MaterialException(std::string_view fmt,
                                   Args &&...args) noexcept
            : PluginException("[Plugin > Material]: ", fmt,
                              std::forward<Args>(args)...) {
        }

        explicit MaterialException(const std::string_view msg) noexcept
            : PluginException("[Plugin > Material]: ", msg) {
        }
    };
}  // namespace raytracer::exception
