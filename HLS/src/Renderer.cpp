#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class Renderer {
  public:
    Renderer() : prng() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<sfp_11_22>& a, vec3<sfp_11_22>& b, vec3<sfp_11_22>& cross, sfp_11_22& dot, vec3<sfp_11_22>& reflect, sfp_11_22 rand_nums[128]) {
        cross = a.cross(b);
        dot = a.dot(b);
        reflect = a.reflect(b);
        for (int i = 0; i < 128; i++) {
            rand_nums[i] = prng.rand();
        }
    }

  private:
    Rand<sfp_11_22, 22, 22, 17, 3, 2> prng;
};