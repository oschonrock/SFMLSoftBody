#pragma once
#include <concepts>

template <typename T>
requires std::floating_point<T> || std::integral<T>
struct averager {
    T sum{};
    T avg{};

    unsigned       sample_count = 0;
    const unsigned max_samples;

    explicit averager(unsigned max_samples_) : max_samples(max_samples_) {}

    T operator()(T new_value) {
        if (sample_count != max_samples) {
            sum += new_value;
            ++sample_count;
        } else {
            sum += new_value - avg;
        }
        avg = sum / sample_count;
        return avg;
    }
};
