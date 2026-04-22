/*
** EPITECH PROJECT, 2026
** IPrimitive
** File description:
** IPrimitive header
*/

#pragma once

namespace object::primitive {

    class IPrimitive {
      public:
        virtual ~IPrimitive() = default;

        bool hits();
    };

}  // namespace object::primitive
