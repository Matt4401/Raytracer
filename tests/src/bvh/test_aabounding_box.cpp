/*
** EPITECH PROJECT, 2026
** test_aabounding_box
** File description:
** Unit tests for AABoundingBox helpers
*/

#include <gtest/gtest.h>

#include "math/AABoundingBox.hpp"

using raytracer::maths::AABoundingBox;
using raytracer::maths::Vector;

TEST(AABOUNDING_BOX, extend_point_initializes_empty_box) {
    AABoundingBox box;

    box.extend(Vector(2.0, 3.0, 4.0));

    EXPECT_DOUBLE_EQ(box.x, 2.0);
    EXPECT_DOUBLE_EQ(box.y, 3.0);
    EXPECT_DOUBLE_EQ(box.z, 4.0);
    EXPECT_DOUBLE_EQ(box.w, 0.0);
    EXPECT_DOUBLE_EQ(box.h, 0.0);
    EXPECT_DOUBLE_EQ(box.d, 0.0);
}

TEST(AABOUNDING_BOX, extend_point_expands_existing_box) {
    AABoundingBox box{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    box.extend(Vector(-2.0, 10.0, 7.0));

    EXPECT_DOUBLE_EQ(box.x, -2.0);
    EXPECT_DOUBLE_EQ(box.y, 2.0);
    EXPECT_DOUBLE_EQ(box.z, 3.0);
    EXPECT_DOUBLE_EQ(box.w, 7.0);
    EXPECT_DOUBLE_EQ(box.h, 8.0);
    EXPECT_DOUBLE_EQ(box.d, 6.0);
}

TEST(AABOUNDING_BOX, pad_expands_box_symmetrically) {
    AABoundingBox box{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    box.pad(2.5);

    EXPECT_DOUBLE_EQ(box.x, -1.5);
    EXPECT_DOUBLE_EQ(box.y, -0.5);
    EXPECT_DOUBLE_EQ(box.z, 0.5);
    EXPECT_DOUBLE_EQ(box.w, 9.0);
    EXPECT_DOUBLE_EQ(box.h, 10.0);
    EXPECT_DOUBLE_EQ(box.d, 11.0);
}