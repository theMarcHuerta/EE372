#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "fixedpoint.h"

using std::sqrt;


// FixedPoint<10, 21> a21 = FixedPoint<10, 21>(3.5)
// Defines a three-dimensional vector used for points, colors, etc.
class vec3 {
  public:
    FixedPoint e[3];  // Stores the three components of the vector.
    double tmpdubs[3];

    // Constructors.
    vec3() : tmpdubs{0,0,0} {
        e[0] = FixedPoint(0);
        e[1] = FixedPoint(0);
        e[2] = FixedPoint(0);
    }  // Default constructor initializes to zero.
    vec3(double e0, double e1, double e2) : tmpdubs{e0,e1,e2} {
        e[0] = FixedPoint(tmpdubs[0]);
        e[1] = FixedPoint(tmpdubs[1]);
        e[2] = FixedPoint(tmpdubs[2]);
    }  // Initializes with specified values.

    // Accessors for the vector components, useful for readability and when using the vector as a point or color.
    FixedPoint x() const { return e[0]; }
    FixedPoint y() const { return e[1]; }
    FixedPoint z() const { return e[2]; }

    // Unary minus returns the negation of the vector, useful for reversing directions.
    vec3 operator-() const { return vec3(-e[0].toDouble(), -e[1].toDouble(), -e[2].toDouble()); }
    // Subscript operators for accessing components by index.
    FixedPoint operator[](int i) const { return e[i]; }
    FixedPoint& operator[](int i) { return e[i]; }

    // Adds the components of another vec3 to this one. Useful for vector addition.
    vec3& operator+=(const vec3 &v) {
        e[0] = e[0] + v.e[0];
        e[1] = e[1] + v.e[1];
        e[2] = e[2] + v.e[2];
        return *this;
    }

    // Multiplies the components by a scalar. Useful for scaling vectors.
    vec3& operator*=(double t) {
        FixedPoint fxd_t = FixedPoint(t);
        e[0] = e[0] * fxd_t;
        e[1] = e[1] * fxd_t;
        e[2] = e[2] * fxd_t;
        return *this;
    }

    // Divides the components by a scalar. Useful for normalizing vectors or scaling them down.
    vec3& operator/=(double t) {
        FixedPoint fxd_t = FixedPoint(t);
        *this = vec3((e[0] / fxd_t).toDouble(), (e[1] / fxd_t).toDouble(), (e[2] / fxd_t).toDouble());
        return *this;
    }

    // Computes the length (magnitude) of the vector. Essential for normalization and distance calculations.
    FixedPoint length() const {
        // std::clog << length_squared().toDouble() << ": V LENGTH_SQUARED\n";
        return FixedPoint(sqrt(length_squared().toDouble()));
    }

    // Computes the square of the length. Faster than length() as it avoids a square root. Useful for comparisons.
    FixedPoint length_squared() const {
        FixedPoint tmp1 = e[0]*e[0];
        FixedPoint tmp2 = e[1]*e[1];
        FixedPoint tmp3 = e[2]*e[2];
        FixedPoint tmp4 = tmp1 + tmp2;
        FixedPoint tmp5 = tmp4 + tmp3;
        return tmp5;
    }

    // Determines if the vector is close to zero in all dimensions. Important for avoiding divide-by-zero errors.
    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(e[0].toDouble()) < s) && (fabs(e[1].toDouble()) < s) && (fabs(e[2].toDouble()) < s);
    }

    // Generates a random vec3. Useful for creating randomized scenes, diffuse materials, etc.
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    // Generates a random vec3 within a specified range. Useful for bounded random values.
    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// Aliasing vec3 as point3 for semantic clarity: points represent positions in space.
using point3 = vec3;

// Vector Utility Functions
// Utility function to output a vec3 to an ostream, typically used for debugging.
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0].toDouble() << ' ' << v.e[1].toDouble() << ' ' << v.e[2].toDouble();
}

// Adds two vec3 vectors and returns the result. Useful for position, direction, and color calculations.
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3((u.e[0] + v.e[0]).toDouble(), (u.e[1] + v.e[1]).toDouble(), (u.e[2] + v.e[2]).toDouble());
}

// Subtracts vector v from vector u and returns the result. Useful for vector calculations like finding a ray direction.
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3((u.e[0] - v.e[0]).toDouble(), (u.e[1] - v.e[1]).toDouble(), (u.e[2] - v.e[2]).toDouble());
}

// Multiplies two vec3 vectors component-wise and returns the result. This can represent color blending.
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3((u.e[0] * v.e[0]).toDouble(), (u.e[1] * v.e[1]).toDouble(), (u.e[2] * v.e[2]).toDouble());
}

// Multiplies a vec3 vector by a scalar and returns the result. Useful for scaling vectors.
inline vec3 operator*(double t, const vec3 &v) {
    return vec3((FixedPoint(t)*v.e[0]).toDouble(), (FixedPoint(t)*v.e[1]).toDouble(), (FixedPoint(t)*v.e[2]).toDouble());
}

// Equivalent to the previous function, demonstrating commutativity of scalar multiplication.
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// Divides a vec3 vector by a scalar (performs the multiplication by the reciprocal of the scalar).
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// Computes the dot product of two vec3 vectors. Fundamental in calculating angles between vectors and for many shading calculations.
inline FixedPoint dot(const vec3 &u, const vec3 &v) {
    return (u.e[0] * v.e[0]) + (u.e[1] * v.e[1]) + (u.e[2] * v.e[2]);
}

// Computes the cross product of two vec3 vectors. Useful for finding a vector perpendicular to the plane defined by two vectors.
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3((u.e[1] * v.e[2] - u.e[2] * v.e[1]).toDouble(),
                (u.e[2] * v.e[0] - u.e[0] * v.e[2]).toDouble(),
                (u.e[0] * v.e[1] - u.e[1] * v.e[0]).toDouble());
}

// Normalizes a vec3 vector, scaling it to a length of 1. This is often required when dealing with directions.
inline vec3 unit_vector(vec3 v) {
    // std::clog << v.length().toDouble() << ": V LENGTH\n";
    return v / v.length().toDouble();
}

// Utility function to generate a random point inside a unit sphere. Frequently used in generating diffuse materials.
inline vec3 random_in_unit_sphere() {
    // while (true) {
    //     auto p = vec3::random(-1,1);
    //     if (p.length_squared() < 1) return p;
    // }
    double xi1 = random_double(); // Uniformly distributed from 0 to 1
    double xi2 = random_double(); // Uniformly distributed from 0 to 1

    double theta = 2.0 * 3.1415926 * xi1; // Uniform distribution from 0 to 2π
    double phi = std::acos(2.0 * xi2 - 1.0); // Correctly distribute from -1 to 1 and acos

    double xs = std::sin(phi) * std::cos(theta); // Note the switch of phi and theta here
    double ys = std::sin(phi) * std::sin(theta);
    double zs = std::cos(phi);

    return vec3(xs, ys, zs);
}

// Generates a random unit vector. Useful in simulating diffuse reflections.
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// Generates a random vector in the same hemisphere as a given normal vector. Important for hemisphere sampling in lighting.
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal).toDouble() > 0.0)  // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

// Reflects a vector v about a normal n. Essential in simulating specular reflections.
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n).toDouble()*n;
}

// Refracts a vector uv through a surface with normal n, according to Snell's law. Key in simulating transparent materials.
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n).toDouble(), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - (r_out_perp.length_squared()).toDouble())) * n;
    return r_out_perp + r_out_parallel;
}

#endif
