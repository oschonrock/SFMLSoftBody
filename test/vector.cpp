#include "Vector2.hpp"
#include "gtest/gtest.h"


TEST(vector2, constructor) {
    EXPECT_EQ(Vec2(), Vec2(0, 0));
}
TEST(vector2, addition) {
    EXPECT_EQ(Vec2I(1,2) + Vec2I(10,20), Vec2(11, 22));
}
// int main() {
//     std::cout << Vec2(0,1) - Vec2(0,2) << '\n'; // [0, 1]
//     std::cout << (Vec2(0,0) - Vec2(1,1)).mag() << '\n'; // 1.41421
//     std::cout << Vec2(1,1).dot(Vec2(2,2)) << '\n'; // 4
//     std::cout << Vec2(1,1).dot(Vec2(2,-2)) << '\n'; // 0
//     std::cout << Vec2(2,2).norm() << '\n'; // 0

// }
