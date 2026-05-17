/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PolynomialSolver
*/

#include "math/PolynomialSolver.hpp"

namespace raytracer::maths {
    [[nodiscard]] double PolynomialSolver::cbrtReal(const double x) noexcept {
        return (x >= 0.0) ? std::cbrt(x) : -std::cbrt(-x);
    }

    [[nodiscard]] bool PolynomialSolver::isZero(const double v) noexcept {
        return std::fabs(v) < EPSILON;
    }
}  // namespace raytracer::maths
