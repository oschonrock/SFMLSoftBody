#pragma once
#include <concepts>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <typeinfo>

template <typename Value, typename Sum = Value,
          typename Count = std::conditional_t<std::is_signed_v<Value>, int, unsigned>>
requires std::floating_point<Value> || std::integral<Value>
struct smoother {
    Sum   sum_{};
    Value avg_{};

    Count       count_ = 0;
    const Count time_constant_;

    explicit smoother(Count max_samples_) : time_constant_(max_samples_) {}

    Value operator()(Value new_value) {
        if (count_ != time_constant_) {
            sum_ += new_value;
            ++count_;
        } else {
            sum_ += new_value - avg_; // correct, and well defined, even with unsigned types
        }
        // count's signedness has been matched to sum and avg
        // must accept that, eg float has fewer sf than int, on many architectures
        avg_ = sum_ / static_cast<Sum>(count_);
        return avg_;
    }
};
