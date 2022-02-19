#pragma once

#include <cmath>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

// Dampens a "noisy" value using "approx exponential damping".
// Submit samples using `operator()`, which returns the damped value.
// MUST choose a SUM type which can hold: sample avg * time_constant
template <typename Value, typename Sum = Value,
          typename Count = std::conditional_t<std::is_signed_v<Sum>, short, unsigned short>>
requires ((std::floating_point<Value> || std::integral<Value>) &&
          (std::floating_point<Sum> || std::integral<Sum>)
          && std::integral<Count>)
class damper {
  public:
    // having given the user a lot of choice on types, we now apply some fairly
    // strict rules to check their choices were sensible
    static_assert(std::is_signed_v<Value> == std::is_signed_v<Sum>,
                  "Value and Sum types must have the same signedness");

    static_assert(std::is_signed_v<Sum> == std::is_signed_v<Count>,
                  "Sum and Count types must have the same signedness");

    static_assert(std::is_floating_point_v<Value> == std::is_floating_point_v<Sum>,
                  "Sum and Value types must both be floating point or both be integral.");

    static_assert(sizeof(Sum) >= sizeof(Value), "Sum type should be at least as big as Value type");
    static_assert(sizeof(Sum) >= sizeof(Count), "Sum type should be at least as big as Count type");

    explicit damper(Count time_constant) : time_constant_(time_constant) {
        if (time_constant <= 0) throw std::invalid_argument("damper needs positive time constant");
    }

    Value operator()(Value sample) noexcept {
        if (std::isnan(sample)) return damped_value_; // ignore it

        if (count_ != time_constant_) {
            // branch avoids distortions for the first time_constant samples
            sum_ += sample;
            ++count_;
        } else {
            // Main running branch: once system is "primed" with time_constant number of samples
            // provides "approxinately exponential damping" with the given time constant

            // correct, and well defined, even with unsigned types
            // but is potential remains for overflow, if user choses a Sum type which cannot
            // hold sample avg * time_constant

            // working around gcc vageries with -Wconversion by doing this in two steps
            // https://stackoverflow.com/a/71183117/1087626
            sum_ += sample;
            sum_ -= damped_value_;
        }

        // only use the static_cast when it should be legitimately needed
        if constexpr (sizeof(Sum) > sizeof(Value)) {
            // static_cast reduces Sum back to Value if different. "Should fit" after the
            // division.
            damped_value_ = static_cast<Value>(sum_ / count_);
        } else {
            // this may cause a -Wconversion warning on gcc if -fsanitize=undefined is passed
            // https://stackoverflow.com/q/71181566/1087626
            damped_value_ = sum_ / count_;
        }
        return damped_value_;
    }

    [[nodiscard]] Value current() const { return damped_value_; }

    void reset() noexcept {
        sum_          = Sum{};
        count_        = Count{};
        damped_value_ = Value{};
    }

  private:
    Sum         sum_{};
    Value       damped_value_{};
    Count       count_{};
    const Count time_constant_;
};
