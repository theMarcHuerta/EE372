#ifndef MEMCONTROLLER_H
#define MEMCONTROLLER_H

#include "RTcore.h"
#include "ray.h"
#include "QuadHit.h"

class MemController
{
public:
    MemController(){}
    
    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<ray> &ray_in,
                        ac_channel<buffer_obj_count> &obj_count_in,
                        ac_channel<quad_hittable> &quad_out1, 
                        ac_channel<buffer_obj_count> &obj_count_out,
                        ac_channel<ray> &ray_out
                        )
    {
        quad_hittable tmp_quad;
        ray tmp_ray;
        buffer_obj_count tmp_obj_count = obj_count_in.read();;

        for (int i = 0; i < MAX_QUADS_IN_BUFFER; i++) {
            tmp_quad = quads_in.read();

            quad_out1.write(tmp_quad);

            if (i == tmp_obj_count.num_quads) break;
        }
        
        tmp_ray = ray_in.read();

        ray_out.write(tmp_ray);

        obj_count_out.write(tmp_obj_count); // write idxs adn params
    }

};

#endif
