#ifndef INTERSECLOOP_H
#define INTERSECLOOP_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"
#include "QuadHit.h"

class IntersecLoop {
public:
    IntersecLoop(){}
    // Function to find the closest hit from channels of quads
    #pragma hls_design interface
    // #pragma hls_pipeline_init_interval 8
    void CCS_BLOCK(hit)(
                        ac_channel<quad_hittable>& quads,
                        ac_channel<ray> &ray_temp_in,
                        ac_channel<uint_11> &quad_max_in,
                        ac_channel<bool> &quad_hit_anything_out,
                        ac_channel<HitRecord> &rec_quad_out,
                        ac_channel<ac_fixed<47, 17, true>> &closest_so_far_out
                        ) 
    {

    ac_fixed<47, 17, true> closest_so_far = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance
    
    ray ray_temp = ray_temp_in.read();
    uint_11 quad_max = quad_max_in.read();

    bool quad_hit_anything = false;
    HitRecord rec_quad = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
    

    #pragma hls_pipeline_init_interval 4
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
    quad_hit_anything_out.write(quad_hit_anything);
    rec_quad_out.write(rec_quad);
    closest_so_far_out.write(closest_so_far);

    }

private:
    QuadHit quadInters;
};


#endif