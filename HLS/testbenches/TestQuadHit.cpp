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
  QuadHit<sfp_11_22> quad_hit;
  public:
    TestQuadHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ray<sfp_11_22>>& ray_channel, ac_channel<_quad_hittable<sfp_11_22>>& quad_channel, ac_channel<HitRecord<sfp_11_22>>& rec_channel) {

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
            sfp_11_22 csf = 2e11; 
            HitRecord<sfp_11_22> rec;

            #ifndef __SYNTHESIS__
            while (qchan.available(1))
            #endif
            {
                // read in quad
                _quad_hittable<sfp_11_22> q = qchan.read();
                quad_hit.run(r, csf, q, rec);
            }

            rec_channel.write(rec);

        }
    }

  private:
    ray<sfp_11_22> r;
    _quad_hittable<sfp_11_22> quads[17];
    ac_channel<_quad_hittable<sfp_11_22>> qchan;

};

#endif
