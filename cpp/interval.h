/*
representing a range of values between a minimum and maximum. It's useful in various 
parts of the ray tracing engine, particularly in managing hit distances for rays, 
where you need to keep track of the nearest hit within a certain range.
*/
#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
  public:
    double min, max;  // Represents the minimum and maximum values of the interval.

    // Default constructor initializes the interval to an "empty" state.
    interval() : min(+infinity), max(-infinity) {}

    // Constructor with specific min and max values.
    interval(double _min, double _max) : min(_min), max(_max) {}

    interval(const interval& a, const interval& b)
    : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    // Checks if a value is within the interval [min, max].
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Checks if a value is strictly inside the interval (min, max).
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Clamps a value to the interval, ensuring it lies within [min, max].
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    // Predefined intervals representing "empty" and "universe" (all-encompassing) cases.
    static const interval empty, universe;
};

// Definitions for the static members `empty` and `universe`.
const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}

#endif
