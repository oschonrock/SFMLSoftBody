#include "Vector2.hpp"
#include "gtest/gtest.h"
#include <limits>
#include <numbers>

using std::numbers::sqrt2;
static constexpr double inv_sqrt2 = sqrt2 / 2.0;

TEST(vector2, constructor) { // NOLINT
    EXPECT_EQ(Vec2(), Vec2(0, 0));
}
TEST(vector2, addition) { // NOLINT
    EXPECT_EQ(Vec2I(1,2) + Vec2I(10,20), Vec2I(11, 22));
}
TEST(vector2, substraction) { // NOLINT
    EXPECT_EQ(Vec2I(10,20) - Vec2I(1,2), Vec2I(9, 18));
}
TEST(vector2, multiplyByScalar) { // NOLINT
    EXPECT_EQ(Vec2I(-4, 4) * 8, Vec2I(-32, 32));
}
TEST(vector2, divideByScalar) { // NOLINT
    EXPECT_EQ(Vec2I(32, -32) / 4, Vec2I(8, -8));
}
TEST(vector2, mag) { // NOLINT
    EXPECT_EQ(Vec2(1,1).mag(), sqrt2);
}
TEST(vector2, dotProduct) { // NOLINT
    EXPECT_EQ(Vec2(1,1).dot(Vec2(2,2)), 4.0);
    EXPECT_EQ(Vec2(1,1).dot(Vec2(2,-2)), 0.0);
}
TEST(vector2, normalizedUnitVector) { // NOLINT
    auto norm   = Vec2(2, 2).norm();
    auto answer = Vec2(inv_sqrt2, inv_sqrt2);
    EXPECT_NEAR(norm.x, answer.x, std::numeric_limits<double>::epsilon());
    EXPECT_NEAR(norm.y, answer.y, std::numeric_limits<double>::epsilon());
}
