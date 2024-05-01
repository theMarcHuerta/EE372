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
  QuadHit quad_hit;
  public:
    TestQuadHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ray>& ray_channel, ac_channel<quad_hittable>& quad_channel, ac_channel<HitRecord>& rec_channel) {

        ac_int<16, false> num_quads = 17;        

        #ifndef __SYNTHESIS__
        while (ray_channel.available(1))
        #endif
        {

            // read in ray and reset variables
            r = ray_channel.read();
            ac_fixed<47, 17, true> csf = LONGEST_DISTANCE; 
            HitRecord rec = {{0, 0, 0}, {0, 0, 0}, 0, 0, 0, {0, 0, 0}};

            // for every ray, read in all quads and run the hit routine
            for (int i = 0; i < 17; i++) {
                q = quad_channel.read();
                
                bool is_hit;
                quad_hit.run(r, csf, q, rec, is_hit);
            }

            rec_channel.write(rec);
        }
    }

  private:
    ray r;
    quad_hittable q;
    // quad_hittable quads[17];
};

#endif
