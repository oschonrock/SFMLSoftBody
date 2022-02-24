#pragma once

#include "Vector2.hpp"
#include <vector>

template <typename Number>
struct Line {
    const Vector2<Number> start;
    const Vector2<Number> end;
    const double          len;  // used for performance caching (sqrt is expensive)
    const Vector2<Number> norm; // more performance caching

    Line(const Vector2<Number>& start_, const Vector2<Number>& end_) // NOLINT similar params
        : start(start_), end(end_), len((end - start).mag()), norm((end - start) / len) {}

    friend std::ostream& operator<<(std::ostream& os, const Line& l) {
        return os << l.start << " => " << l.end << " (" << l.len << ")";
    }
};

