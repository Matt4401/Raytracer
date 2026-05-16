/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PolynomialSolver
*/

#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>

namespace raytracer::maths {
    struct PolynomialResult {
        int count;
        std::array<double, 4> roots;
    };

    class PolynomialSolver {
      public:
        static constexpr double EPSILON = 1e-12;

        static int solveCubic(double a, double b, double c, double d,
                              std::array<double, 3>& roots) noexcept;
        static int solveQuartic(double a, double b, double c, double d,
                                double e,
                                std::array<double, 4>& roots) noexcept;
        static int solveQuadratic(double a, double b, double c, double& r0,
                                  double& r1) noexcept;
        static int solveLinear(double a, double b, double& r0) noexcept;

      private:
        [[nodiscard]] static double cbrtReal(double x) noexcept;
        [[nodiscard]] static bool isZero(double v) noexcept;
    };

    inline int PolynomialSolver::solveLinear(const double a, const double b,
                                             double& r0) noexcept {
        if (isZero(a))
            return 0;
        r0 = -b / a;
        return 1;
    }

    inline int PolynomialSolver::solveQuadratic(const double a, const double b,
                                                const double c, double& r0,
                                                double& r1) noexcept {
        if (isZero(a)) {
            double tmp = 0.0;
            const int n = solveLinear(b, c, tmp);
            r0 = tmp;
            return n;
        }

        const double disc = b * b - 4.0 * a * c;

        if (disc < -EPSILON)
            return 0;
        if (std::fabs(disc) <= EPSILON) {
            r0 = r1 = -b / (2.0 * a);
            return 1;
        }
        const double sqrtDisc = std::sqrt(std::max(0.0, disc));
        const double q =
            (b >= 0.0) ? -0.5 * (b + sqrtDisc) : -0.5 * (b - sqrtDisc);

        r0 = q / a;
        r1 = (std::fabs(q) > EPSILON) ? (c / q) : r0;
        return 2;
    }

    inline int PolynomialSolver::solveCubic(
        const double a, const double b, const double c, const double d,
        std::array<double, 3>& roots) noexcept {
        if (isZero(a)) {
            double r0 = 0.0, r1 = 0.0;
            const int n = solveQuadratic(b, c, d, r0, r1);
            if (n >= 1)
                roots[0] = r0;
            if (n >= 2)
                roots[1] = r1;
            return n;
        }

        const double invA = 1.0 / a;
        const double B = b * invA;
        const double C = c * invA;
        const double D = d * invA;
        const double p = C - B * B / 3.0;
        const double q = (2.0 * B * B * B - 9.0 * B * C + 27.0 * D) / 27.0;
        const double shift = -B / 3.0;
        const double disc4 = -4.0 * p * p * p - 27.0 * q * q;

        if (disc4 > EPSILON) {
            const double m = 2.0 * std::sqrt(-p / 3.0);
            const double arg = 3.0 * q / (2.0 * p) * std::sqrt(-3.0 / p);
            const double theta = std::acos(std::clamp(arg, -1.0, 1.0));

            roots[0] = shift + m * std::cos(theta / 3.0);
            roots[1] =
                shift + m * std::cos((theta + 2.0 * std::numbers::pi) / 3.0);
            roots[2] =
                shift + m * std::cos((theta + 4.0 * std::numbers::pi) / 3.0);
            return 3;
        }

        if (disc4 < -EPSILON) {
            const double sqrtDisc = std::sqrt(std::max(0.0, -disc4 / 108.0));
            const double u3 = -q / 2.0 + sqrtDisc;
            const double v3 = -q / 2.0 - sqrtDisc;
            const double u = cbrtReal(u3);
            const double v = cbrtReal(v3);

            roots[0] = shift + u + v;
            return 1;
        }

        if (isZero(q)) {
            roots[0] = shift;
            return 1;
        }
        roots[0] = shift + 3.0 * q / p;
        roots[1] = shift - 3.0 * q / (2.0 * p);
        return 2;
    }

    inline int PolynomialSolver::solveQuartic(
        const double a, const double b, const double c, const double d,
        const double e, std::array<double, 4>& roots) noexcept {
        if (isZero(a)) {
            std::array<double, 3> r3{};
            const int n = solveCubic(b, c, d, e, r3);
            for (int i = 0; i < n; ++i) roots[i] = r3[i];
            return n;
        }

        const double invA = 1.0 / a;
        const double B = b * invA;
        const double C = c * invA;
        const double D = d * invA;
        const double E = e * invA;
        const double B2 = B * B;
        const double p = C - 3.0 * B2 / 8.0;
        const double q = D + B2 * B / 8.0 - B * C / 2.0;
        const double r =
            E - 3.0 * B2 * B2 / 256.0 + B2 * C / 16.0 - B * D / 4.0;
        const double shift = -B / 4.0;

        if (isZero(q)) {
            double u0 = 0.0, u1 = 0.0;
            const int nu = solveQuadratic(1.0, p, r, u0, u1);
            int count = 0;
            auto trySquareRoot = [&](const double u) {
                if (u < -EPSILON)
                    return;
                const double sq = std::sqrt(std::max(0.0, u));
                roots[count++] = shift + sq;
                if (sq > EPSILON)
                    roots[count++] = shift - sq;
            };
            if (nu >= 1)
                trySquareRoot(u0);
            if (nu >= 2)
                trySquareRoot(u1);
            return count;
        }

        std::array<double, 3> yRoots{};
        const int nCubic = solveCubic(8.0, -4.0 * p, -8.0 * r,
                                      4.0 * p * r - q * q, yRoots);  // ferrari
        if (nCubic <= 0)
            return 0;

        double y = yRoots[0];
        for (int i = 1; i < nCubic; ++i)
            if (yRoots[i] > y)
                y = yRoots[i];

        const double radicand = 2.0 * y - p;
        if (radicand < -EPSILON)
            return 0;

        const double alpha = std::sqrt(std::max(0.0, radicand));
        double c1, c2;

        if (alpha < EPSILON) {
            const double inner = y * y - r;
            if (inner < -EPSILON)
                return 0;
            const double sq = std::sqrt(std::max(0.0, inner));
            c1 = y + sq;
            c2 = y - sq;
        } else {
            const double bv = q / (2.0 * alpha);
            c1 = y + bv;
            c2 = y - bv;
        }

        int count = 0;
        double r0 = 0.0, r1 = 0.0;
        const int n1 = solveQuadratic(1.0, -alpha, c1, r0, r1);

        if (n1 >= 1)
            roots[count++] = shift + r0;
        if (n1 >= 2)
            roots[count++] = shift + r1;
        const int n2 = solveQuadratic(1.0, alpha, c2, r0, r1);
        if (n2 >= 1)
            roots[count++] = shift + r0;
        if (n2 >= 2)
            roots[count++] = shift + r1;
        return count;
    }
}  // namespace raytracer::maths