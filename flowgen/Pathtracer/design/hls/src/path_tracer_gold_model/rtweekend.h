/*
Serves as a global include file that brings together common dependencies, constants, 
and utility functions needed across different parts of the ray tracing project. By 
centralizing these elements, it helps in reducing code duplication and improving 
maintainability.

Key Components:
Common Constants: Defines universally required constants like pi and infinity, which
are used in various mathematical calculations throughout the project.

Utility Functions: Provides essential utility functions such as random_double for 
generating random numbers, which are crucial for implementing features like 
anti-aliasing and diffuse material scattering.

Common Includes: Aggregates commonly used headers so that they can be included in 
other files through a single include of rtweekend.h, simplifying code structure 
and compilation dependencies.
*/

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>



// Usings
using std::shared_ptr;    // Allows for the use of smart pointers without specifying the namespace.
using std::make_shared;   // Simplifies the creation of smart pointers.
using std::sqrt;          // Provides access to the square root function.

// Constants
const double infinity = std::numeric_limits<double>::infinity();  // A representation of infinity.
const double pi = 3.1415926535897932385;  // The value of Pi for use in conversions and calculations.

// Utility Functions
inline double degrees_to_radians(double degrees) {
    // Converts degrees to radians for angle measurements.
    return degrees * pi / 180.0;
}


inline uint32_t xorshift32(uint32_t &state) {
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

inline double random_double() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 375821;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return static_cast<double>(result) / static_cast<double>(UINT32_MAX);

}

inline double random_double1() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 375821;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return static_cast<double>(result) / static_cast<double>(UINT32_MAX);

}

inline double random_double2() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 39251088;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return static_cast<double>(result) / static_cast<double>(UINT32_MAX);

}

// Common Headers
#include "interval.h"  // Includes the definition for interval data structure used for hit records.
#include "ray.h"       // Includes the ray definition used throughout the ray tracing code.
#include "vec3.h"      // Includes the vec3 class for 3D vector and point operations.

#endif
