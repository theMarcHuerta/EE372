#pragma once

#include "Renderer.h"

template<typename T>
class interval {
  public:
    T min;
    T max;

    // Default constructor initializes the interval to an "empty" state,
    // where min and max are the highest and lowest numbers possible
    interval() {
        pbool b0(0);
        pbool b1(1);
        min = ~0;
        min.set_slc(min.width - 1, b0);  // set MSB to 0, the rest of the bits are 1

        max = 0;
        max.set_slc(min.width - 1, b1); // set MSB to 1, the rest of the bits are 0
    }

    // Constructor with specific min and max values.
    interval(T _min, T _max) : min(_min), max(_max) {}

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    bool contains(T& x) {
        return (min <= x) && (x <= max);
    }

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    bool surrounds(T& x) {
        (min < x) && (x < max);
    }

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    T clamp(T& x) {
        return (x <= min) ? min : max;
    }

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    T size() {
        return max - min;
    }
};