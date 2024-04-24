#ifndef TESTRAND_H
#define TESTRAND_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/Rand.h"

#pragma hls_design top
class TestRand {
  public:
    TestRand() : rand_val(), rand_vec() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(sfp_11_22 rand_nums[32], vec3<sfp_11_22>& a, vec3<sfp_11_22>& b) {
        for (int i = 0; i < 32; i++) {
            rand_val.run(rand_nums[i]);
        }
        rand_vec.run(a);
        rand_vec.run(b);
    }

  private:
    Rand_val<sfp_11_22> rand_val;
    Rand_unit_vec<sfp_11_22> rand_vec;
};

#endif
