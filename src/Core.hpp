/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#ifndef CORE_HPP_
#define CORE_HPP_

#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {
    class Core {
      public:
        Core() = default;
        ~Core() = default;

        void init();
        void run();

      private:
        PluginManager _plugManager;
        ObjectFactory _objFactory;
    };
}  // namespace raytracer

#endif /* !CORE_HPP_ */
