/*
** EPITECH PROJECT, 2026
** DlLoader
** File description:
** DlLoader header
*/

#pragma once

#include <dlfcn.h>

#include <exception>
#include <filesystem>
#include <string_view>

#include "exception/PluginException.hpp"

namespace raytracer {
    class DlLoader {
      public:
        DlLoader() = default;
        ~DlLoader() {
            this->close();
        };

        DlLoader(DlLoader &&other) noexcept : _handler(other._handler) {
            other._handler = nullptr;
        }

        DlLoader &operator=(DlLoader &&other) noexcept {
            if (this != &other) {
                this->close();
                this->_handler = other._handler;
                other._handler = nullptr;
            }
            return *this;
        }
        DlLoader(const DlLoader &) = delete;
        DlLoader &operator=(const DlLoader &) = delete;

        /**
         * @brief Open and load a shared library
         *
         * @param path: Path to the shared library file
         * @return true if library was successfully loaded, false otherwise
         */
        bool open(const std::filesystem::path &path) {
            void *buffer = dlopen(path.c_str(), RTLD_LAZY);
            if (buffer == nullptr) {
                return false;
            }
            this->close();
            this->_handler = buffer;
            return true;
        }

        /**
         * @brief Close and unload the shared library
         */
        void close() {
            if (this->_handler != nullptr) {
                dlclose(this->_handler);
                this->_handler = nullptr;
            }
        }

        /**
         * @brief Load and execute a function from the loaded library
         *
         * @tparam T The return type of the function to retrieve and execute
         * @param funcName The name of the function symbol to load and call
         * @return The result of executing the function, cast to type T
         * @throw raytracer::exception::PluginException if the symbol is not
         * found or library is not loaded
         */
        template <typename T>
        T get(const std::string_view &funcName) const {
            if (this->_handler == nullptr) {
                throw raytracer::exception::PluginException(
                    "DlLoader: Plugin is not loaded");
            }

            dlerror();
            void *sym = dlsym(this->_handler, funcName.data());
            const char *error = dlerror();
            if (error != nullptr || sym == nullptr) {
                throw raytracer::exception::PluginException(
                    "DlLoader: ", "Entry \"{}\" unfound", funcName);
            }
            auto func = reinterpret_cast<T (*)()>(sym);
            return func();
        };

      private:
        void *_handler = nullptr;
    };
}  // namespace raytracer
