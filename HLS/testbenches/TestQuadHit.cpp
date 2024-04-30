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

        ac_int<16, false> q_idx = 0;
        // load quads into array
        #ifndef __SYNTHESIS__
        while (quad_channel.available(1))
        #endif
        {
            quads[q_idx] = quad_channel.read();
        }

        #ifndef __SYNTHESIS__
        while (ray_channel.available(1))
        #endif
        {

            // populate new channel
            for (int i = 0; i < 17; i++) {
                qchan.write(quads[i]);
            }

            // read in ray and reset variables
            r = ray_channel.read();
            ac_fixed<47, 17, true> csf = LONGEST_DISTANCE; 
            HitRecord rec;

            #ifndef __SYNTHESIS__
            while (qchan.available(1))
            #endif
            {
                // read in quad
                quad_hittable q = qchan.read();
                bool is_hit;
                quad_hit.run(r, csf, q, rec, is_hit);
            }

            rec_channel.write(rec);

        }
    }

  private:
    ray r;
    quad_hittable quads[17];
    ac_channel<quad_hittable> qchan;

};

#endif
