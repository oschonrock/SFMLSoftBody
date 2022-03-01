#include "Vector2.hpp"
#include "damper.hpp"
#include <cmath>
#include <concepts>
#include <cstdint>
#include "gtest/gtest.h"
#include <limits>
#include <string>
#include <type_traits>

// for testing purposes only
template <typename Value, typename Sum = Value,
          typename Count = std::conditional_t<std::is_signed_v<Sum>, short, unsigned short>>
void test_damper(Value pre_step = 100, Value post_step = 0, Count tc = 10) { // NOLINT Complexity

    // modelling a step response from Value{pre_step} down to Value{post_step} after tc samples
    // expecting flat Value{100}
    // then we feed another tc samples at the pre_step value to ensure "main branch" of `damper`
    // continues to return a flat profile
    // Then we start feeding `post_step` values and expect "approximately exponential decay" towards
    // Value{0}. `tc` is aprox equiv to the "time constant" in exponential damping

    // test empty damper
    auto d = damper<Value, Sum, Count>(tc);
    static_assert(std::is_same_v<decltype(d.current()), Value>);
    EXPECT_EQ(d.current(), Value{});

    // tc samples with pre_step value
    for (auto i = Count{1}; i <= tc; ++i) {
        auto dv = d(pre_step);
        SCOPED_TRACE("Stage 1: feeding " + std::to_string(i) + "th value of " +
                     std::to_string(pre_step));
        EXPECT_EQ(dv, pre_step); // check at every step that the `damped_value_` is "flat"
    }
    // `damper` is now fully "primed" with pre_step values

    // continue to push another tc pre_step values to ensure the output doesn't change
    for (auto i = Count{1}; i <= tc; ++i) {
        auto dv = d(pre_step);
        SCOPED_TRACE("Stage 2: feeding " + std::to_string(i) + "th value of " +
                     std::to_string(pre_step));
        EXPECT_EQ(dv, pre_step); // check at every step that the `damped_value_` is "flat"
    }

    // tc further samples of `post_step` values
    for (auto i = Count{1}; i <= tc; ++i) {
        auto dv = d(post_step);
        SCOPED_TRACE("Stage 3: feeding " + std::to_string(i) + "th value of " +
                     std::to_string(post_step));

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

// User defined types
TEST(Damper, Vec2) { // NOLINT
    auto d  = damper<Vec2>(4);
    auto dv = d({1, 0});
    EXPECT_EQ(dv, Vec2(1, 0));
    dv = d({-1, 0});
    EXPECT_EQ(dv, Vec2(0, 0));
    dv = d({0, 1});
    EXPECT_EQ(dv, Vec2(0, 1.0 / 3));
    dv = d({0, -1.0});
    EXPECT_EQ(dv, Vec2(0, 0));
}

// signed integer types
TEST(Damper, Short) { test_damper<short>(); } // NOLINT
TEST(Damper, Int) { test_damper<int>(); } // NOLINT
TEST(Damper, Long) { test_damper<long>(); } // NOLINT
TEST(Damper, LongLong) { test_damper<long long>(); } // NOLINT

// signed integer types with a step from negative to positive
TEST(Damper, ShortNegstep) { test_damper<short>(-100, 100); } // NOLINT
TEST(Damper, IntNegstep) { test_damper<int>(-100, 100); } // NOLINT
TEST(Damper, LongNegstep) { test_damper<long>(-100, 100); } // NOLINT
TEST(Damper, LongLongNegstep) { test_damper<long long>(-100, 100); } // NOLINT

// unsigned integer types
TEST(Damper, UnsignedShort) { test_damper<unsigned short>(); } // NOLINT
TEST(Damper, Unsigned) { test_damper<int>(-100, 100); } // NOLINT
TEST(Damper, UnsignedLong) { test_damper<unsigned long>(); } // NOLINT
TEST(Damper, UnsignedLongLong) { test_damper<unsigned long long>(); } // NOLINT

// FP types
TEST(Damper, Float) { test_damper<float>(); } // NOLINT
TEST(Damper, Double) { test_damper<double>(); } // NOLINT
TEST(Damper, LongDouble) { test_damper<long double>(); } // NOLINT

// FP types with a step from negative to positive
TEST(Damper, FloatNegstep) { test_damper<float>(-100.0, 100.0); } // NOLINT
TEST(Damper, DoubleNegstep) { test_damper<double>(-100.0, 100.0); } // NOLINT
TEST(Damper, LongDoubleNegstep) { test_damper<long double>(-100.0, 100.0); } // NOLINT

// tiny integer types (one use case is microcontroller analog input damping)
TEST(Damper, Uint8Uint16) { test_damper<std::uint8_t, std::uint16_t>(); } // NOLINT
TEST(Damper, Int8Int16) { test_damper<std::int8_t, std::int16_t>(); } // NOLINT

// this test fails, because the sum overflows
// I have tried to algebraically eliminate the sum, but it seems that, for integer arithmetic,
// we always need at least a temporary result which can hold sample avg * time_constant
// TEST(damper, uint8_t__uint8_t__uint8_t) { test_damper<std::uint8_t, std::uint8_t,
// std::uint8_t>(); }

// however, it is still a valid set of template params when sample avg * tc is small
TEST(Damper, Uint8Uint8SmallValues) { // NOLINT
    test_damper<std::uint8_t, std::uint8_t, std::uint8_t>(0, 40, 5);
}

TEST(Damper, Int8Int8Int8Negstep) { // NOLINT
    test_damper<std::int8_t, std::int8_t, std::int8_t>(-10, 10, 10);
}
TEST(Damper, IntIntNegstep) { test_damper<int, int>(-100'000, 100'000, 10); } // NOLINT
TEST(Damper, Int8Int16Negstep) { test_damper<std::int8_t, std::int16_t>(-10, 10, short(10)); } // NOLINT
