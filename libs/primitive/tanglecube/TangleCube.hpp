/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TangleCube
*/

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class TangleCube : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit TangleCube(const std::map<std::string, std::any> &args);
        explicit TangleCube(const maths::Vector &center,
                            double constant = 11.8);
        explicit TangleCube(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, double constant = 11.8);

        ~TangleCube() override = default;

        const double &constant() const noexcept;

        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      private:
        static maths::Vector computeNormal(const maths::Vector &localPoint);

        double _constant;

        static constexpr std::size_t EXPECTED_ARGS = 3;
    };

}  // namespace raytracer::object::primitive