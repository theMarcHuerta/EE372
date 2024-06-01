#ifndef LOOPDISTRIB_H
#define LOOPDISTRIB_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"

#endif
class LoopDistrib {
public:
    LoopDistrib(){}
    // Function to find the closest hit from channels of quads
    #pragma hls_design interface
    // #pragma hls_pipeline_init_interval 8
    void CCS_BLOCK(run)(ac_channel<ray>& ray_in,
                        ac_channel<buffer_obj_count> &params_in,
                        ac_channel<quad_hittable>& quads,
                        ac_channel<rgb_in> &attenuation_chan_in,
                        ac_channel<rgb_in> &accumalated_color_chan_in,

                        ac_channel<rgb_in> &attenuation_chan_out,
                        ac_channel<rgb_in> &accumalated_color_out,

                        ac_channel<ray> &ray_out_loopone,
                        ac_channel<ray> &ray_out_looptwo,
                        ac_channel<ray> &ray_out_worldhit,

                        ac_channel<quad_hittable>& quad_out_loopone,
                        ac_channel<quad_hittable>& quad_out_looptwo,
                        ac_channel<uint_11> &quad_max_outone,
                        ac_channel<uint_11> &quad_max_outtwo,

                        ac_channel<buffer_obj_count> &params_out
                        ) 
    {
        buffer_obj_count tmp_params = {0,{0,0,0}, false};
        tmp_params = params_in.read();
        params_out.write(tmp_params);

        ray ray_temp = ray_in.read(); 
        ray_out_loopone.write(ray_temp);
        ray_out_looptwo.write(ray_temp);
        ray_out_worldhit.write(ray_temp);

        attenuation_chan_out.write(accumalated_color_chan_in.read());
        accumalated_color_out.write(attenuation_chan_in.read());

        bool quad_hit_anything;
        bool quad_hit_anything_two;

        uint_11 quad_max_one = (tmp_params.num_quads) >> 1;
        uint_11 quad_max_two = tmp_params.num_quads - quad_max_one;

        #pragma hls_pipeline_init_interval 2
        for (int i = 0; i < MAX_QUADS_IN_BUFFER_HALF; i++) {
            quad_out_loopone.write(quads.read());
            quad_out_looptwo.write(quads.read());
            if (i == (quad_max_one - 1)) break;
        }
        // if we have an odd number of quads
        if(quad_max_two[0] == 1){
            quad_out_looptwo.write(quads.read());
        }
        
        // WE CAN HAVE THE READS HAPPEN IN PARALLEL THEN DECIDE WHICH ONE WAS THE CLOSEST OUT OF ALL QUADS
        // MIGHT BE TOO MUCH AREA AND RELIES ON EVEN SPLIT SO PROBABLY NOT

        quad_max_outone.write(quad_max_one);
        quad_max_outtwo.write(quad_max_two);
    }

};