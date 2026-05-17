/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** FractalBuilder
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "fractal/IFractalStrategy.hpp"
#include "object/IObject.hpp"

namespace raytracer::object::primitive {

    std::shared_ptr<IFractalStrategy> buildMandelbulbStrategy(
        const std::map<std::string, std::any> &params,
        const std::string &contextName);

    IObject *buildMandelbulb(const std::map<std::string, std::any> &params,
                             const std::string &contextName);

}  // namespace raytracer::object::primitive