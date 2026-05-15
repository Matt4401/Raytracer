/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder
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
    class Cylinder : public APrimitive {
      public:
        using object::primitive::IPrimitive::hits;

        explicit Cylinder(const std::map<std::string, std::any> &args);
        explicit Cylinder(const maths::Vector &center, double radius);
        explicit Cylinder(
            std::shared_ptr<raytracer::object::material::IMaterial> material,
            const maths::Vector &center, double radius);

        ~Cylinder() override = default;

        const double &radius() const noexcept;
        bool hits(const maths::Ray &ray, HitRecord &record) const override;
        AABoundingBox boundingBox() override;
        SurfaceData surfaceData(const HitRecord &record) const override;

      protected:
        virtual bool validateConstraints(const maths::Vector &p) const {
            return true;
        }

      private:
        double _radius;

        static constexpr std::size_t EXPECTED_ARGS = 3;
    };
}  // namespace raytracer::object::primitive
