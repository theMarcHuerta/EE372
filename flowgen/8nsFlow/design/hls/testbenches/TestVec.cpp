#ifndef TESTVEC_H
#define TESTVEC_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"

#pragma hls_design top
class TestVec {
  Vec3_cross<sfp_11_22> cross_i;
  Vec3_dot<sfp_11_22> dot_i;
  Vec3_reflect<sfp_11_22> reflect_i;
  Ray_at<sfp_11_22> at_i;
  public:
    TestVec() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<sfp_11_22>& a, vec3<sfp_11_22>& b, ray<sfp_11_22>& r, vec3<sfp_11_22>& cross, sfp_11_22& dot, vec3<sfp_11_22>& reflect, vec3<sfp_11_22>& ray_pos) {
        sfp_11_22 t = 12.5;
        cross_i.run(a, b, cross);
        dot_i.run(a, b, dot);
        reflect_i.run(a, b, reflect);
        at_i.run(r, t, ray_pos);
    }
};

#endif
