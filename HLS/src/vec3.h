#pragma once

#include <ac_math.h>
#include <ac_fixed.h>
#include <ac_int.h>

// Defines a three-dimensional vector used for points, colors, etc.
template<typename T>
class vec3 {    
  public:
    T x;
    T y;
    T z;  

    // Constructors.
    vec3() : x(0), y(0), z(0) {}  // Default constructor initializes to zero.
    vec3(T e0, T e1, T e2) : x(e0), y(e1), z(e2) {}  // Initializes with specified values.

    // Unary minus returns the negation of the vector, useful for reversing directions.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> negate() {
      return vec3<T>(-x, -y, -z);
    }

    // Adds the components of another vec3 to this one. Useful for vector addition.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> add(vec3<T>& v) {
      return vec3<T>(x + v.x, y + v.y, z + v.z);
    }

    // Adds the components of another vec3 to this one. Useful for vector addition.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> sub(vec3<T>& v) {
      return vec3<T>(x - v.x, y - v.y, z - v.z);
    }

    // Multiplies the components by a scalar. Useful for scaling vectors.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> mult(T& t) {
      return vec3<T>(x * t, y * t, z * t);
    }

    // Multiplies the components by a vector.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> mult(vec3<T>& v) {
      return vec3<T>(x * v.x, y * v.y, z * v.z);
    }

    // Divides the components by a scalar.
    #pragma hls_design ccore
    vec3<T> div(T& t) {
      return vec3<T>(x / t, y / t, z / t);
    }

    // Divides the components by a vector.
    #pragma hls_design ccore
    vec3<T> div(vec3<T>& v) {
      return vec3<T>(x / v.x, y / v.y, z / v.z);
    }

    // Computes the square of the length. Faster than length() as it avoids a square root. Useful for comparisons.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    T length_squared() {
      return x*x + y*y + z*z;
    }

    // Computes the length (magnitude) of the vector. Essential for normalization and distance calculations.
    #pragma hls_design ccore
    T length() {
      T l_sq = length_squared();
      T result;

      // unsigned variables for ac_sqrt()
      ac_fixed<l_sq.width-1, l_sq.i_width-1, false> u_l_sq = l_sq;   // TODO: optimize bit widths
      ac_fixed<l_sq.width-1, l_sq.i_width-1, false> u_result;

      ac_math::ac_sqrt(u_l_sq, u_result);
      result = u_result;  // convert back to signed
      return result;
    }

    // Computes the dot product of two vec3 vectors. Fundamental in calculating angles between vectors and for many shading calculations.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    T dot(vec3<T>& v) {
      return x * v.x + y * v.y + z * v.z;
    }

    // Computes the cross product of two vec3 vectors. Useful for finding a vector perpendicular to the plane defined by two vectors.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> cross(vec3<T>& v) {
      return vec3(y * v.z - z * v.y,
                  z * v.x - x * v.z,
                  x * v.y - y * v.x);
    }

    // Normalizes a vec3 vector, scaling it to a length of 1. This is often required when dealing with directions.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3<T> unit() {
      vec3<T> v(x, y, z);
      T length = v.length();
      return v.div(length);
    }

    // Reflects this vector about a normal n. Essential in simulating specular reflections.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    vec3 reflect(vec3<T>& n) {
      vec3<T> v(x, y, z);
      T dot_result = v.dot(n);
      vec3<T> first_mult = n.mult(dot_result);
      T c = 2;
      vec3<T> second_mult = first_mult.mult(c);
      return v.sub(second_mult);
    }

//   // // Refracts a vector uv through a surface with normal n, according to Snell's law. Key in simulating transparent materials.
//   // inline vec3 refract(const vec3& uv, const vec3& n, T etai_over_etat) {
//   //     auto cos_theta = fmin(dot(-uv, n), 1.0);
//   //     vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
//   //     vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
//   //     return r_out_perp + r_out_parallel;
//   // }

// //     // Determines if the vector is close to zero in all dimensions. Important for avoiding divide-by-zero errors.
// //     bool near_zero() const {
// //         const auto s = 1e-8;
// //         return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
// //     }

// //     // Generates a random vec3. Useful for creating randomized scenes, diffuse materials, etc.
// //     static vec3 random() {
// //         return vec3(random_ac_fixed<T>(), random_ac_fixed<T>(), random_ac_fixed<T>());
// //     }

// //     // Generates a random vec3 within a specified range. Useful for bounded random values.
// //     static vec3 random(T min, T max) {
// //         return vec3(random_ac_fixed<T>(min,max), random_ac_fixed<T>(min,max), random_ac_fixed<T>(min,max));
// //     }
// // };

// // // Utility function to generate a random point inside a unit sphere. Frequently used in generating diffuse materials.
// // inline vec3 random_in_unit_sphere() {
// //     while (true) {
// //         auto p = vec3::random(-1,1);
// //         if (p.length_squared() < 1) return p;
// //     }
// // }

// // // Generates a random unit vector. Useful in simulating diffuse reflections.
// // inline vec3 random_unit_vector() {
// //     return unit_vector(random_in_unit_sphere());
// // }

// // // Generates a random vector in the same hemisphere as a given normal vector. Important for hemisphere sampling in lighting.
// // inline vec3 random_on_hemisphere(const vec3& normal) {
// //     vec3 on_unit_sphere = random_unit_vector();
// //     if (dot(on_unit_sphere, normal) > 0.0)  // In the same hemisphere as the normal
// //         return on_unit_sphere;
// //     else
// //         return -on_unit_sphere;
// // }

};