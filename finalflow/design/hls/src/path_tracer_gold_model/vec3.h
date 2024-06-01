#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "rtweekend.h"

using std::sqrt;

// Defines a three-dimensional vector used for points, colors, etc.
class cpp_vec3 {
  public:
    double e[3];  // Stores the three components of the vector.

    // Constructors.
    cpp_vec3() : e{0,0,0} {}  // Default constructor initializes to zero.
    cpp_vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}  // Initializes with specified values.

    // Accessors for the vector components, useful for readability and when using the vector as a point or color.
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Unary minus returns the negation of the vector, useful for reversing directions.
    cpp_vec3 operator-() const { return cpp_vec3(-e[0], -e[1], -e[2]); }
    // Subscript operators for accessing components by index.
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Adds the components of another cpp_vec3 to this one. Useful for vector addition.
    cpp_vec3& operator+=(const cpp_vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Multiplies the components by a scalar. Useful for scaling vectors.
    cpp_vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Divides the components by a scalar. Useful for normalizing vectors or scaling them down.
    cpp_vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    // Computes the length (magnitude) of the vector. Essential for normalization and distance calculations.
    double length() const {
        return sqrt(length_squared());
    }

    // Computes the square of the length. Faster than length() as it avoids a square root. Useful for comparisons.
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Determines if the vector is close to zero in all dimensions. Important for avoiding divide-by-zero errors.
    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

};

// Aliasing cpp_vec3 as point3 for semantic clarity: points represent positions in space.
using point3 = cpp_vec3;

// Vector Utility Functions
// Utility function to output a cpp_vec3 to an ostream, typically used for debugging.
inline std::ostream& operator<<(std::ostream &out, const cpp_vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Adds two cpp_vec3 vectors and returns the result. Useful for position, direction, and color calculations.
inline cpp_vec3 operator+(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtracts vector v from vector u and returns the result. Useful for vector calculations like finding a ray direction.
inline cpp_vec3 operator-(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Multiplies two cpp_vec3 vectors component-wise and returns the result. This can represent color blending.
inline cpp_vec3 operator*(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Multiplies a cpp_vec3 vector by a scalar and returns the result. Useful for scaling vectors.
inline cpp_vec3 operator*(double t, const cpp_vec3 &v) {
    return cpp_vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// Equivalent to the previous function, demonstrating commutativity of scalar multiplication.
inline cpp_vec3 operator*(const cpp_vec3 &v, double t) {
    return t * v;
}

// Divides a cpp_vec3 vector by a scalar (performs the multiplication by the reciprocal of the scalar).
inline cpp_vec3 operator/(cpp_vec3 v, double t) {
    return (1/t) * v;
}

// Computes the dot product of two cpp_vec3 vectors. Fundamental in calculating angles between vectors and for many shading calculations.
inline double dot(const cpp_vec3 &u, const cpp_vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Computes the cross product of two cpp_vec3 vectors. Useful for finding a vector perpendicular to the plane defined by two vectors.
inline cpp_vec3 cross(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Normalizes a cpp_vec3 vector, scaling it to a length of 1. This is often required when dealing with directions.
inline cpp_vec3 unit_vector(cpp_vec3 v) {
    return v / v.length();
}

// Utility function to generate a random point inside a unit sphere. Frequently used in generating diffuse materials.
inline cpp_vec3 random_in_unit_sphere() {
    // while (true) {
    //     auto p = cpp_vec3::random(-1,1);
    //     if (p.length_squared() < 1) return p;
    // }
    float xi1 = random_double(); // Uniformly distributed from 0 to 1
    float xi2 = random_double(); // Uniformly distributed from 0 to 1

    float theta = 2.0 * 3.1415926 * xi1; // Uniform distribution from 0 to 2π
    float phi = std::acos(2.0 * xi2 - 1.0); // Correctly distribute from -1 to 1 and acos

    float xs = std::sin(phi) * std::cos(theta); // Note the switch of phi and theta here
    float ys = std::sin(phi) * std::sin(theta);
    float zs = std::cos(phi);

    return cpp_vec3(xs, ys, zs);
}

// Generates a random unit vector. Useful in simulating diffuse reflections.
inline cpp_vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

#endif
