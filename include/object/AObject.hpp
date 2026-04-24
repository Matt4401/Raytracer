/*
** EPITECH PROJECT, 2026
** `
** File description:
** AObject
*/

#ifndef AOBJECT_HPP_
#define AOBJECT_HPP_

#include "IObject.hpp"

namespace raytracer::object {
    class AObject : public IObject {
      public:
        AObject(Type type) : _type(type) {};
        ~AObject() override = default;
        Type type() const override {
            return _type;
        };

      protected:
        Type _type;
    };
}  // namespace raytracer::object

#endif /* !AOBJECT_HPP_ */
