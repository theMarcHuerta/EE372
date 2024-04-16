#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class TestVec {
  public:
    TestVec() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<sfp_11_22>& a, vec3<sfp_11_22>& b, ray<sfp_11_22>& r, vec3<sfp_11_22>& cross, sfp_11_22& dot, vec3<sfp_11_22>& reflect, vec3<sfp_11_22>& ray_pos) {
        sfp_11_22 t = 12.5;
        cross = a.cross(b);
        dot = a.dot(b);
        reflect = a.reflect(b);
        ray_pos = r.at(t);
    }
};