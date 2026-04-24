/*
** EPITECH PROJECT, 2026
** IPrimitive
** File description:
** IPrimitive header
*/

#pragma once

#include "Vector3"

namespace raytracer::object::primitive {

    class IPrimitive {
      public:
        struct BoundingBox {
            double x;
            double y;
            double z;
            double w;
            double h;
            double d;
        };

        virtual ~IPrimitive() = default;
        virtual bool hits() = 0;
        virtual BoundingBox boundingBox() = 0;
    };

}  // namespace raytracer::object::primitive
