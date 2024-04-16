#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class TestRand {
  public:
    TestRand() : prng() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(sfp_11_22 rand_nums[16], vec3<sfp_11_22>& a, vec3<sfp_11_22>& b) {
        for (int i = 0; i < 16; i++) {
            rand_nums[i] = prng.rand();
        }
        a = prng.rand_unit_vec();
        b = prng.rand_unit_vec();
    }

  private:
    Rand<sfp_11_22, 22, 22, 17, 3, 2> prng;
};