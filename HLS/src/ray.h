#pragma once

#include "RTcore.h"

template<typename T>
struct ray {
  vec3<T> orig; 
  vec3<T> dir;
};

template<typename T>
class Ray_at {
  Vec3_mult_s<T> mult;
  Vec3_add<T> add;
  public:
    // Calculates a point along the ray at parameter t.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(ray<T>& r, T& t, vec3<T>& out) {
        vec3<T> mult_result;
        mult.run(r.dir, t, mult_result);
        add.run(r.orig, mult_result, out);
    }
};
    