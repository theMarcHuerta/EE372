#ifndef TESTWORLDHIT_H
#define TESTWORLDHIT_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/WorldHit.h"

#pragma hls_design top
class TestWorldHit {

  public:
    TestWorldHit() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ray>& ray_in,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<quad_hittable>& quads,
             ac_channel<rgb_in> &attenuation_chan_in,
             ac_channel<rgb_in> &accumalated_color_chan_in,
             ac_channel<rgb_in> &attenuation_chan_out,
             ac_channel<rgb_in> &accumalated_color_out,
             ac_channel<HitRecord>& hit_out,
             ac_channel<ray> &ray_out,
             ac_channel<bool> &isHit) {
        
        world_hit.hit(ray_in, params_in, quads, attenuation_chan_in, accumalated_color_chan_in, 
                        attenuation_chan_out, accumalated_color_out, hit_out, ray_out, isHit);
    }

    private:

        WorldHit world_hit;
};

#endif