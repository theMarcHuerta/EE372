#ifndef TESTQUADHIT_H
#define TESTQUADHIT_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/QuadHit.h"

#pragma hls_design top
class TestQuadHit {

  public:
    TestQuadHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ray& r, ac_fixed<47, 17, true>& closest_so_far, quad_hittable& quad, HitRecord& rec, bool& hitWorld) {
        QuadHit quad_hit;
        quad_hit.run(r, closest_so_far, quad, rec, hitWorld);
    }

};

#endif