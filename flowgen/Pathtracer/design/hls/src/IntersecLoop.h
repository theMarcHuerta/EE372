#ifndef INTERSECLOOP_H
#define INTERSECLOOP_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"
#include "QuadHit.h"

#endif
class IntersecLoop {
public:
    IntersecLoop(){}
    // Function to find the closest hit from channels of quads
    #pragma hls_design
    // #pragma hls_pipeline_init_interval 8
    void CCS_BLOCK(hit)(ac_channel<quad_hittable>& quads,
                        ray &ray_temp,
                        uint_11 &quad_max,
                        ac_fixed<47, 17, true> &closest_so_far,
                        bool &quad_hit_anything,
                        HitRecord &rec_quad
                        ) 
    {
    #pragma hls_pipeline_init_interval 18
    for (int i = 0; i < MAX_QUADS_IN_BUFFER_HALF; i++) {
        quad_hittable quad = quads.read();
        HitRecord temp_rec = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
        bool hitWorld = false;
        quadInters.run(ray_temp, closest_so_far, quad, temp_rec, hitWorld);
        if (hitWorld) {
            quad_hit_anything = true;
            rec_quad = temp_rec;
        }
        if (i == (quad_max- 1)) break;
    }
    }

private:
    QuadHit quadInters;
};