#ifndef RAY_H
#define RAY_H

#include "RTcore.h"
#include "vec3.h"

template<typename T1, typename T2>
class Ray_at {
  Vec3_mult_s<ac_fixed<34, 11, true>, T1, ac_fixed<34+t.wdith, 11+t.i_width, true>> mult;
  Vec3_add<ac_fixed<21, 11, true>, ac_fixed<34+T1.wdith, 11+T1.i_width, true>, T2> add;
  public:
    // Calculates a point along the ray at parameter t.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(ray& r, T1& t, vec3<T2>& out) {
        vec3<ac_fixed<34+t.wdith, 11+t.i_width, true>> mult_result; // sized to be result of ray dir and width of input
        mult.run(r.dir, t, mult_result);
        add.run(r.orig, mult_result, out); // sized for ray origin// outshould be dir + t2 width + 1
    }
};

#endif
