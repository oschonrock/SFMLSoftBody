#include "median.hpp"
#include "Vector2.hpp"
#include "gtest/gtest.h"
#include <list>
#include <vector>

TEST(median, medianOdd) { // NOLINT
    EXPECT_EQ(median(std::vector<int>{9, 8, 7, 5, 6, 4, 3, 2, 1}), 5);
}
TEST(median, medianInPlaceOdd) { // NOLINT
    std::vector<int> ints{9, 8, 7, 5, 6, 4, 3, 2, 1};
    std::vector<int> tmp_ints = ints;
    EXPECT_EQ(median_in_place(tmp_ints), 5);
    EXPECT_NE(ints, tmp_ints);
}
TEST(median, medianEven) { // NOLINT
    EXPECT_EQ(median(std::vector<int> {40, 10, 20, 30}), 25);
    EXPECT_EQ(median(std::vector<int>{1, 4, 2, 3}), 2);
    EXPECT_EQ(median(std::vector<double>{10, 9, 5, 6, 8, 7, 4, 3, 2, 1}), 5.5);
}
TEST(median, medianInPlaceEven) { // NOLINT
    std::vector<int> ints{9, 8, 7, 5, 10, 6, 4, 3, 2, 1};
    std::vector<int> tmp_ints = ints;
    EXPECT_EQ(median_in_place(tmp_ints), 5);
    EXPECT_NE(ints, tmp_ints);

    std::vector<double> doubles{10, 9, 5, 6, 8, 7, 4, 3, 2, 1};
    std::vector<double> tmp_doubles = doubles;
    EXPECT_EQ(median_in_place(tmp_doubles), 5.5);
    EXPECT_NE(doubles, tmp_doubles);
}
TEST(median, medianVec2Mag) { // NOLINT
    EXPECT_EQ(median(std::list<Vec2>{{9, 8}, {7, 6}, {3, 4}, {3, 2}, {0, 1}}, {}, &Vec2::mag), 5);
}
  // {
  //   std::vector<int> ints{9, 8, 7, 6, 5, 4, 3, 2, 1};
  //   std::cout << "median_in_place(ints) = " << median_in_place(ints) << "\n";
  // }
  // {
  // }
  // {
  //   std::vector<vec2> vec2s{{9, 8}, {7, 6}, {5, 4}, {3, 2}};
  //   std::cout << "median_in_place(mag(vec2)) using lambda = "
  //             << median_in_place(vec2s, {}, [](const auto& v) { return v.mag(); }) << "\n";
  // }
  // {
  //   std::vector<vec2> vec2s{{9, 8}, {7, 6}, {5, 4}, {3, 2}};
  //   std::cout << "median_in_place(mag(vec2) using iters) = "
  //             << median_in_place(vec2s.begin(), vec2s.end(), {},
  //                                [](const auto& v) { return v.mag(); })
  //             << "\n";
  // }
