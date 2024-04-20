#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"

#pragma hls_design top
class TestQuadHit {
  QuadHit<sfp_11_22> quad_hit;
  public:
    TestQuadHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ray<sfp_11_22>& r, sfp_11_22& closest_so_far, quad_hittable& quad, HitRecord<sfp_11_22>& rec) {
        quad_hit.run(r, closest_so_far, quad, rec);
    }
};