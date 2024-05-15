#ifndef TESTSPHEREHIT_H
#define TESTSPHEREHIT_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/SphHit.h"

#pragma hls_design top
class TestSphereHit {
  SphereHit<sfp_11_22> sphere_hit;
  public:
    TestSphereHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ray<sfp_11_22>& r, sfp_11_22& closest_so_far, sphere_hittable& sphere, HitRecord<sfp_11_22>& rec) {
        sphere_hit.run(r, closest_so_far, sphere, rec);
    }
};

#endif