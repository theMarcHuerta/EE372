#ifndef TESTSCATTER_H
#define TESTSCATTER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/LambertianScatter.h"

#pragma hls_design top
class TestLambertian {

  public:
    TestLambertian() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(HitRecord& rec, ray& scattered) {
        scatterlamb.run(rec, scattered);
    } 

  private:
    LambertianScatter scatterlamb;

};

#endif