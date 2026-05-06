/*
** EPITECH PROJECT, 2026
** IPrimitive
** File description:
** IPrimitive header
*/

#pragma once

#include <any>
#include <map>
#include <optional>
#include <string>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/ReflTypes.hpp"

namespace raytracer::object::primitive {
    struct MaterialProperties {
        maths::Color color;
        maths::Vector emission;
        RefltT reflType;
        double reflectivity;
        double transparency;
        double ior;
        double roughness;
        double metalness;
        std::optional<maths::Vector> perturbedNormal;
    };

    struct SurfaceData {
        maths::Vector normal;
        maths::Vector uv;
        std::map<std::string, std::any> extraParams;
        MaterialProperties material;
    };

    struct HitContext {
        double distance;
        maths::Vector hitPoint;
        SurfaceData surfaceData;
    };

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
        /**
         * @brief Compute ray-primitive intersection and return complete hit
         * context.
         * @param ray the ray to test for intersection with the primitive
         * @param computeSurfaceData if true, compute normal, uv, and material;
         * if false, return only distance and hitPoint with empty surfaceData
         * @return HitContext containing distance, hit point, and surface data
         * if intersection occurs, std::nullopt otherwise
         */
        virtual std::optional<HitContext> hits(const maths::Ray &ray,
                                               bool computeSurfaceData = true) = 0;

        /**
         * @return a BoundingBox struct that defines the axis-aligned bounding
         * box of the primitive. The bounding box is defined by its position (x,
         * y, z) and its dimensions (w, h, d). The position represents the
         * coordinates of the minimum corner of the bounding box in 3D space,
         * while the dimensions represent the width (w), height (h), and depth
         * (d) of the bounding box along the x, y, and z axes, respectively. The
         * bounding box is used for efficient spatial partitioning and collision
         * detection in ray tracing algorithms, allowing for faster intersection
         * tests by quickly determining if a ray intersects with the bounding
         * box before performing more complex intersection calculations with the
         * primitive itself.
         */
        virtual BoundingBox boundingBox() = 0;

        virtual const std::string &name() const noexcept = 0;
        virtual maths::Vector center() const noexcept = 0;
    };

}  // namespace raytracer::object::primitive
