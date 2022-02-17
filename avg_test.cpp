#include "damper.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

// for testing purposes only
template <typename Value, typename Sum = Value,
          // type switch is required to prevent warnings AND incorrect arithmetic with -ves
          typename Count = std::conditional_t<std::is_signed_v<Value>, int, unsigned>>
requires std::floating_point<Value> || std::integral<Value>
void test_damper(Value pre_step = 100, Value post_step = 0, Count tc = 10) {

    // modelling a step response from Value{pre_step} down to Value{post_step} after tc samples
    // expecting flat Value{100} and then "approximately exponential decay" towards Value{0}
    // `tc` is aprox equiv to the "time constant" in exponential damping

    // test empty damper
    auto d = damper<Value, Sum, Count>(tc);
    static_assert(std::is_same_v<decltype(d.current()), Value>);
    EXPECT_EQ(d.current(), Value{});

    // tc samples with pre_step value
    for (auto i = Count{1}; i <= tc; ++i) {
        auto dv = d(pre_step);
        static_assert(std::is_same_v<decltype(dv), Value>);
        EXPECT_EQ(dv, pre_step); // check at every step that the `damped_value_` is "flat"
    }
    // `damper` is now fully "primed" with pre_step values

    // tc further samples of `post_step` values
    for (auto i = Count{1}; i <= tc; ++i) {
        auto dv = d(post_step);
        static_assert(std::is_same_v<decltype(dv), Value>);

        // check at every sample that we are matching the exponential decay curve
        // which predicts "approximately exponential damping"
        auto expected = post_step + (pre_step - post_step) * std::pow((tc - 1.0L) / tc, i);

        if constexpr (std::is_integral_v<Value>) {
            auto predicted = static_cast<Value>(std::round(expected));
            // allow an integer result to be "off-by-one" after rounding
            // std::abs can't be used here, because programm would be ill-formed for some types
            EXPECT_TRUE(dv - predicted == Value{0} || dv - predicted == Value{1} ||
                        predicted - dv == Value{1});
        } else {
            auto predicted = static_cast<Value>(expected);
            // for FP we expect the prediction to be within appropiately scaled epsilon
            EXPECT_LT(std::fabs(dv - predicted),
                      std::fabs(pre_step - post_step) * std::numeric_limits<Value>::epsilon());
        }
    }
}

// signed integer types
TEST(damper, short) { test_damper<short>(); }
TEST(damper, int) { test_damper<int>(); }
TEST(damper, long) { test_damper<long>(); }
TEST(damper, long_long) { test_damper<long long>(); }

// signed integer types with a step from negative to positive
TEST(damper, short_negstep) { test_damper<short>(-100, 100); }
TEST(damper, int_negstep) { test_damper<int>(-100, 100); }
TEST(damper, long_negstep) { test_damper<long>(-100, 100); }
TEST(damper, long_long_negstep) { test_damper<long long>(-100, 100); }

// unsigned integer types
TEST(damper, unsigned_short) { test_damper<unsigned short>(); }
TEST(damper, unsigned) { test_damper<unsigned>(); }
TEST(damper, unsigned_long) { test_damper<unsigned long>(); }
TEST(damper, unsigned_long_long) { test_damper<unsigned long long>(); }

// FP types
TEST(damper, float) { test_damper<float>(); }
TEST(damper, double) { test_damper<double>(); }
TEST(damper, long_double) { test_damper<long double>(); }

// FP types with a step from negative to positive
TEST(damper, float_negstep) { test_damper<float>(-100.0, 100.0); }
TEST(damper, double_negstep) { test_damper<double>(-100.0, 100.0); }
TEST(damper, long_double_negstep) { test_damper<long double>(-100.0, 100.0); }

// tiny integer types (one use case is microcontroller analog input damping)
TEST(damper, uint8_t__uint16_t) { test_damper<std::uint8_t, std::uint16_t>(); }
TEST(damper, uint8_t__uint16_t__uint8_t) {
    test_damper<std::uint8_t, std::uint16_t, std::uint8_t>();
}

// this test fails, because the sum overflows
// I have tried to algebraically eliminate the sum, but it seems that, for integer arithmetic,
// we always need at least a temporary result which can hold sample avg * time_constant
// TEST(damper, uint8_t__uint8_t__uint8_t) { test_damper<std::uint8_t, std::uint8_t,
// std::uint8_t>(); }

// however, it is still a valid set of template params when sample avg * tc is small
TEST(damper, uint8_t__uint8_t__uint8_t_small_values) {
    test_damper<std::uint8_t, std::uint8_t, std::uint8_t>(0, 40, 5);
}
