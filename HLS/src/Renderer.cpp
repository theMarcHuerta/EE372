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
    void CCS_BLOCK(run)(vec3<fxp_32>& a, vec3<fxp_32>& b, vec3<fxp_32>& cross, fxp_32& dot, vec3<fxp_32>& reflect, fxp_32 rand_nums[128]) {
        cross = a.cross(b);
        dot = a.dot(b);
        reflect = a.reflect(b);
        for (int i = 0; i < 128; i++) {
            rand_nums[i] = prng.next();
        }
    }

  private:
    Rand<fxp_32, 23, 22> prng;
};