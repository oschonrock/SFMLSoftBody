#pragma once

#include <concepts>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <typeinfo>

// Dampens a "noisy" value using "approx exponential damping".
// Submit samples using `operator()`, which returns the damped value.
// MUST choose a SUM type which can hold: sample avg * time_constant
template <typename Value, typename Sum = Value,
          // type switch is required to prevent warnings AND incorrect arithmetic with -ves
          typename Count = std::conditional_t<std::is_signed_v<Value>, short, unsigned short>>
requires std::floating_point<Value> || std::integral<Value>
class damper {
  public:
    explicit damper(Count time_constant) : time_constant_(time_constant) {}

    Value operator()(Value sample) {
        if (count_ != time_constant_) {
            // branch avoids distortions for the first time_constant samples
            sum_ += sample;
            ++count_;
        } else {
            // Main running branch: once system is "primed" with time_constant number of samples
            // provides "approxinately exponential damping" with the given time constant

            // correct, and well defined, even with unsigned types
            // but there remains is potential for overflow, if user choses a Sum type which cannot
            // hold sample avg * time_constant
            sum_ += sample - damped_value_;
        }
        // first static_cast suppresses -Wimplicit-int-conversion about very small types being
        // implicitly promoted to integer then demoted back again to be assigned to a small Value
        // type.

        // second static_cast is about a -Wimplicit-int-float-conversion of count_ from
        // eg (unsigned) int to float which is never likely to be relevant
        damped_value_ = static_cast<Value>(sum_ / static_cast<Sum>(count_));

        return damped_value_;
    }

    Value current() { return damped_value_; }

    void reset() {
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

