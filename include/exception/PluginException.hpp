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
#include <utility>

#include "exception/Exception.hpp"

namespace raytracer::exception {
    class PluginException : public Exception {
      public:
        static constexpr std::string_view PREFIX = "[Plugin error]: ";
        static constexpr std::string_view PRIMITIVE_PREFIX =
            "[Plugin > Primitive]: ";
        static constexpr std::string_view TRANSFORMATION_PREFIX =
            "[Plugin > Transformation]: ";
        static constexpr std::string_view LIGHT_PREFIX = "[Plugin > Light]: ";
        static constexpr std::string_view MATERIAL_PREFIX =
            "[Plugin > Material]: ";

        template <typename... Args>
        explicit PluginException(std::string_view fmt, Args &&...args)
            : PluginException(PREFIX, fmt, std::forward<Args>(args)...) {
        }

        template <typename... Args>
        explicit PluginException(const std::string_view prefix,
                                 const std::string_view fmt,
                                 Args &&...args)
            : Exception(std::string(prefix) +
                            std::vformat(fmt, std::make_format_args(args...)),
                        source::current()) {
        }

        explicit PluginException(
            const std::string_view prefix, const std::string_view msg,
            const source &location = source::current()) noexcept
            : Exception(std::string(prefix) + std::string(msg), location) {
        }

        explicit PluginException(
            const std::string_view msg,
            const source &location = source::current()) noexcept
            : PluginException(PREFIX, msg, location) {
        }
    };

    class PrimitivesException : public PluginException {
      public:
        template <typename... Args>
        explicit PrimitivesException(std::string_view fmt, Args &&...args)
            : PluginException(PRIMITIVE_PREFIX, fmt,
                              std::forward<Args>(args)...) {
        }

        explicit PrimitivesException(
            const std::string_view msg,
            const source &location = source::current()) noexcept
            : PluginException(PRIMITIVE_PREFIX, msg, location) {
        }
    };

    class TransformationException : public PluginException {
      public:
        template <typename... Args>
        explicit TransformationException(std::string_view fmt, Args &&...args)
            : PluginException(TRANSFORMATION_PREFIX, fmt,
                              std::forward<Args>(args)...) {
        }

        explicit TransformationException(
            const std::string_view msg,
            const source &location = source::current()) noexcept
            : PluginException(TRANSFORMATION_PREFIX, msg, location) {
        }
    };

    class LightException : public PluginException {
      public:
        template <typename... Args>
        explicit LightException(std::string_view fmt, Args &&...args)
            : PluginException(LIGHT_PREFIX, fmt,
                              std::forward<Args>(args)...) {
        }

        explicit LightException(
            const std::string_view msg,
            const source &location = source::current()) noexcept
            : PluginException(LIGHT_PREFIX, msg, location) {
        }
    };

    class MaterialException : public PluginException {
      public:
        template <typename... Args>
        explicit MaterialException(std::string_view fmt, Args &&...args)
            : PluginException(MATERIAL_PREFIX, fmt,
                              std::forward<Args>(args)...) {
        }

        explicit MaterialException(
            const std::string_view msg,
            const source &location = source::current()) noexcept
            : PluginException(MATERIAL_PREFIX, msg, location) {
        }
    private:
        template <typename... Args>
        static std::string formatWithPrefix(std::string_view prefix, std::string_view fmt, Args &&...args) {
            return std::string(prefix) + std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        }
    };
}  // namespace raytracer::exception

