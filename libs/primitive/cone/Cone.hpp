/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone
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
    class Cone : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Cone(const std::map<std::string, std::any> &args);
        explicit Cone(const maths::Vector &center, double openingFactor);
        explicit Cone(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, double openingFactor);

        ~Cone() override = default;

        const double &openingFactor() const noexcept;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      protected:
        virtual bool validateConstraints(const maths::Vector &p) const {
            return true;
        }
        bool solveQuadraticIntersection(const maths::Ray &ray,
                                        HitRecord &record) const;

      private:
        double _openingFactor;
        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}  // namespace raytracer::object::primitive
