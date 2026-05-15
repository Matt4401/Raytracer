/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** APrimitive
*/

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "math/Ray.hpp"
#include "math/Vector.hpp"
#include "object/AObject.hpp"
#include "object/IObject.hpp"
#include "object/material/IMaterial.hpp"
#include "object/primitive/IPrimitive.hpp"

namespace raytracer::object::primitive {
    class APrimitive : public IPrimitive, public AObject {
      public:
        static constexpr AABoundingBox INFINITE_BOUNDING_BOX{-1e6, -1e6, -1e6,
                                                             2e6,  2e6,  2e6};

        explicit APrimitive(
            const std::string_view name, const maths::Vector &center,
            std::shared_ptr<raytracer::object::material::IMaterial> material =
                nullptr)
            : AObject(Type::PRIMITIVE),
              _material(std::move(material)),
              _name(name),
              _center(center) {
        }
        ~APrimitive() override = default;

        bool hits(const maths::Ray &ray, HitRecord &record) const override = 0;
        double hits(const maths::Ray &ray) const override;
        AABoundingBox boundingBox() override = 0;
        SurfaceData surfaceData(const HitRecord &record) const override = 0;

        maths::Vector center() const noexcept override;
        const std::string &name() const noexcept override;

        void setId(int id) override;
        int id() const override;
        void setLimitBox(const maths::AABoundingBox &box) override;
        bool isInfinite() const override;

      protected:
        std::shared_ptr<raytracer::object::material::IMaterial> _material;
        std::string _name;
        maths::Vector _center;
        maths::AABoundingBox _sceneLimit;
        bool _hasLimit = false;
        int _id = -1;
    };
}  // namespace raytracer::object::primitive
