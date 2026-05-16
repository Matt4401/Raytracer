/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Matrix4 tests
*/

#include <gtest/gtest.h>

#include <cmath>

#include "math/Matrix4.hpp"

namespace {
    void expectMatrixNear(const raytracer::maths::Matrix4 &matrix,
                          const raytracer::maths::Matrix4 &expected,
                          double epsilon = 1e-9) {
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                EXPECT_NEAR(matrix.at(row, column), expected.at(row, column),
                            epsilon)
                    << "Mismatch at [" << row << "][" << column << "]";
            }
        }
    }
}  // namespace

TEST(Matrix4, IdentityLeavesPointUnchanged) {
    const raytracer::maths::Matrix4 matrix;
    const raytracer::maths::Vector point(1.0, -2.0, 3.5);

    const auto transformed = matrix.transformPoint(point);

    EXPECT_DOUBLE_EQ(transformed.x, point.x);
    EXPECT_DOUBLE_EQ(transformed.y, point.y);
    EXPECT_DOUBLE_EQ(transformed.z, point.z);
}

TEST(Matrix4, TranslationMovesPointsButNotVectors) {
    const auto matrix = raytracer::maths::Matrix4::translation(
        raytracer::maths::Vector(4.0, -2.0, 1.5));
    const raytracer::maths::Vector point(1.0, 2.0, 3.0);
    const raytracer::maths::Vector direction(1.0, 0.0, -1.0);

    const auto movedPoint = matrix.transformPoint(point);
    const auto movedDirection = matrix.transformVector(direction);

    EXPECT_DOUBLE_EQ(movedPoint.x, 5.0);
    EXPECT_DOUBLE_EQ(movedPoint.y, 0.0);
    EXPECT_DOUBLE_EQ(movedPoint.z, 4.5);
    EXPECT_DOUBLE_EQ(movedDirection.x, direction.x);
    EXPECT_DOUBLE_EQ(movedDirection.y, direction.y);
    EXPECT_DOUBLE_EQ(movedDirection.z, direction.z);
}

TEST(Matrix4, InverseCancelsCompositeTransform) {
    const auto transform = raytracer::maths::Matrix4::translation(
                               raytracer::maths::Vector(4.0, -2.0, 1.0)) *
                           raytracer::maths::Matrix4::rotationZ(M_PI / 2.0) *
                           raytracer::maths::Matrix4::scale(
                               raytracer::maths::Vector(2.0, 3.0, 4.0));
    const auto inverse = transform.inverse();
    const auto product = transform * inverse;

    expectMatrixNear(product, raytracer::maths::Matrix4::identity());
}

TEST(Matrix4, NormalUsesInverseTranspose) {
    const auto transform = raytracer::maths::Matrix4::scale(
        raytracer::maths::Vector(2.0, 3.0, 4.0));
    const raytracer::maths::Vector normal(0.0, 0.0, 1.0);

    const auto transformed = transform.transformNormal(normal);

    EXPECT_DOUBLE_EQ(transformed.x, 0.0);
    EXPECT_DOUBLE_EQ(transformed.y, 0.0);
    EXPECT_DOUBLE_EQ(transformed.z, 1.0);
}