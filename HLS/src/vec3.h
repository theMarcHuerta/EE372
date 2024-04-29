#ifndef VEC3_H
#define VEC3_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "RTcore.h"

template<typename T>
struct vec3 {
  T x;
  T y;
  T z;
};

template<typename T>
class Vec3_negate {
  public:
    // Unary minus returns the negation of the vector, useful for reversing directions.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T>& in, vec3<T>& out) {
      out.x = -in.x;
      out.y = -in.y;
      out.z = -in.z;
    }
};


template<typename T1, typename T2, typename T3>
class Vec3_add {
  public:
    // first number should be put 
    // Adds the components of another vec3 to this one. Useful for vector addition.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, vec3<T2>& b, vec3<T3>& out) {
      out.x = a.x + b.x;
      out.y = a.y + b.y;
      out.z = a.z + b.z;
    }
};

template<typename T1, typename T2, typename T3>
class Vec3_sub {
  public:
    // Adds the components of another vec3 to this one. Useful for vector addition.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, vec3<T2>& b, vec3<T3>& out) {
      out.x = a.x - b.x;
      out.y = a.y - b.y;
      out.z = a.z - b.z;
    }
};

template<typename T1, typename T2, typename T3>
class Vec3_mult_s {
  public:
    // Multiplies the components by a scalar. Useful for scaling vectors.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, T2& b, vec3<T3>& out) {
      out.x = in.x * t;
      out.y = in.y * t;
      out.z = in.z * t;
    }
};

template<typename T1, typename T2, typename T3>
template<typename T>
class Vec3_mult_v {
  public:
    // Multiplies the components by a vector.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, vec3<T2>& b, vec3<T>& out) {
      out.x = a.x * b.x;
      out.y = a.y * b.y;
      out.z = a.z * b.z;
    }
};


template<typename T1, typename T2, typename T3>
class Vec3_div_s {
  public:
    // Divides the components by a scalar.
    #pragma hls_design ccore
    void run(vec3<T1>& in, T2& t, vec3<T3>& out) {
      out.x = in.x / t;
      out.y = in.y / t;
      out.z = in.z / t;
    }
};

template<typename T1, typename T2, typename T3>
class Vec3_div_v {
  public:
    // Divides the components by a vector.
    #pragma hls_design ccore
    void run(vec3<T1>& a, vec3<T2>& b, vec3<T3>& out) {
      out.x = a.x / b.x;
      out.y = a.y / b.y;
      out.z = a.z / b.z;
    }
};

template<typename T1, typename T2>
class Vec3_len_sq {
  public:
    // Computes the square of the length. Faster than length() as it avoids a square root. Useful for comparisons.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& in, T2& out) {
      out = in.x*in.x + in.y*in.y + in.z*in.z;
    }
};

template<typename T>
class Vec3_len {
  Vec3_len_sq<T, ac_fixed<T.width * 2, T.i_width * 2, true>> length_squared;
  public:
    // Computes the length (magnitude) of the vector. Essential for normalization and distance calculations.
    #pragma hls_design ccore
    void run(vec3<T>& in, T& out) {
      ac_fixed<T.width * 2, T.i_width * 2, true> l_sq;
      length_squared.run(in, l_sq);

      // unsigned variables for ac_sqrt()
      ac_fixed<l_sq.width-1, l_sq.i_width-1, false> u_l_sq = l_sq;
      ac_fixed<T.width - 1, T.i_width - 1, false> u_result;

      ac_math::ac_sqrt(u_l_sq, u_result);

      T tmp_result = u_result;
      // convert back to type T
      out = tmp_result;
    }
};

template<typename T, typname T_DivOut>
class Vec3_unit {
  Vec3_len<T> length;
  Vec3_div_s<T, T, T_DivOut> div;
  public:
    // Normalizes a vec3 vector, scaling it to a length of 1. This is often required when dealing with directions.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T>& in, vec3<T_DivOut>& out) {
      T len;
      length.run(in, len);

      div.run(in, len, out);
    }
};

template<typename T1, typename T2, typename T3>
class Vec3_dot {
  public:
    // Computes the dot product of two vec3 vectors. Fundamental in calculating angles between vectors and for many shading calculations.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, vec3<T2>& b, T3& out) {
      out = a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

template<typename T1, typename T2, typename T3>
class Vec3_cross {
  public:
    // Computes the cross product of two vec3 vectors. Useful for finding a vector perpendicular to the plane defined by two vectors.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(vec3<T1>& a, vec3<T2>& b, vec3<T3>& out) {
      out.x = a.y * b.z - a.z * b.y;
      out.y = a.z * b.x - a.x * b.z;
      out.z = a.x * b.y - a.y * b.x;
    }
};


// template<typename T1, typename T2, typename T3>
// class Vec3_reflect {
//   Vec3_dot<T1, T2, ac_fixed<T1.width + T2.width + 2, T1.i_width + T2.i_width + 2, true>> dot; // 2xtra integer bits to account for overflow
//   Vec3_mult_s<T> mult;
//   Vec3_sub<T> sub;
//   public:
//     // Reflects vector v about a normal n. Essential in simulating specular reflections.
//     #pragma hls_design ccore
//     #pragma hls_ccore_type combinational
//     void run(vec3<T1>& v, vec3<T2>& n, vec3<T3>& out) {
//       ac_fixed<T1.width + T2.width + 2, T1.i_width + T2.i_width + 2, true> dot_result;

//       vec3<ac_fixed<dot_result.width + T2.width + 2, dot_result.i_width + T2.i_width + 2, true>> first_mult;
//       ac_fixed<4, 3, true> c = 2;
//       // WHY ARE WE MULTIPLYING BY TWO?? JUST BITSHIFT??
//       vec3<ac_fixed<dot_result.width + T1.width + 2, dot_result.i_width + T1.i_width + 2, true>>> second_mult;

//       dot.run(v, n, dot_result);
//       mult.run(n, dot_result, first_mult);
//       mult.run(first_mult, c, second_mult);
      
//       sub.run(v, second_mult, out);
//     }
// };

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

// // // Utility function to generate a random point inside a unit sphere. Frequently used in generating diffuse materials.
// // inline vec3 random_in_unit_sphere() {
// //     while (true) {
// //         auto p = vec3::random(-1,1);
// //         if (p.length_squared() < 1) return p;
// //     }
// // }

// // // Generates a random vector in the same hemisphere as a given normal vector. Important for hemisphere sampling in lighting.
// // inline vec3 random_on_hemisphere(const vec3& normal) {
// //     vec3 on_unit_sphere = random_unit_vector();
// //     if (dot(on_unit_sphere, normal) > 0.0)  // In the same hemisphere as the normal
// //         return on_unit_sphere;
// //     else
// //         return -on_unit_sphere;
// // }

#endif
