#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"
#include "QuadHit.h"


class WorldHit {
public:
    WorldHit(){}
    // Function to find the closest hit from channels of quads
    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    // #pragma hls_interface ap_ctrl_none port=return
    void CCS_BLOCK(hit)(ac_channel<ray>& ray_in,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<quad_hittable>& quads,
             ac_channel<rgb_in> &attenuation_chan_in,
             ac_channel<rgb_in> &accumalated_color_chan_in,
             ac_channel<rgb_in> &attenuation_chan_out,
             ac_channel<rgb_in> &accumalated_color_out,
             ac_channel<HitRecord>& hit_out,
             ac_channel<ray> &ray_out,
             ac_channel<bool> &isHit
             ) 
    {
    #ifndef __SYNTHESIS__
    while(params_in.available(1))
    #endif
    {
        buffer_obj_count tmp_params;
        tmp_params = params_in.read();

        ray ray_temp;
        ray_temp = ray_in.read(); 

        rgb_in tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in tmp_accum_in;
        tmp_accum_in = attenuation_chan_in.read();

        bool quad_hit_anything = false;
        ac_fixed<47, 17, true> closest_so_far_quad = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance

        // WE CAN HAVE THE READS HAPPEN IN PARALLEL THEN DECIDE WHICH ONE WAS THE CLOSEST OUT OF ALL QUADS
        // MIGHT BE TOO MUCH AREA AND RELIES ON EVEN SPLIT SO PROBABLY NOT
        
        // PROBABLY COMBINE LOOPS TO SERIALIZE AND NOT HAVE TO KEEP TRACK OF 2 VARIABLES

        for (int i = 0; i < MAX_QUADS_IN_BUFFER; i++) {
            quad_hittable quad = quads.read();
            HitRecord temp_rec;
            bool hitWorld;
            quadInters.run(ray_temp, closest_so_far_quad, quad, temp_rec, hitWorld);
            if (hitWorld) {
                quad_hit_anything = true;
                rec_quad = temp_rec;
            }
            if (i == (tmp_params.num_quads - 1)) break;
        }

        if (quad_hit_anything) {
            hit_out.write(rec_quad);  // Write the closest hit record to the output channel
            isHit.write(true);
            accumalated_color_out.write(tmp_color_in);
        }
        else {
            rgb_in colorMulOut;

            colorMulOut.r = tmp_accum_in.r * tmp_params.background.r;
            colorMulOut.g = tmp_accum_in.g * tmp_params.background.g;
            colorMulOut.b = tmp_accum_in.b * tmp_params.background.b;

            rgb_in colorAddOut;

            colorAddOut.r = colorMulOut.r + tmp_color_in.r;
            colorAddOut.g = colorMulOut.g + tmp_color_in.g;
            colorAddOut.b = colorMulOut.b + tmp_color_in.b;

            accumalated_color_out.write(colorAddOut);
            isHit.write(false);
            hit_out.write(rec_quad);  // doesnt really matter which one we write out as long as we write somethign out
        }
        //any ray going out is not a camera ray anymore
        ray_temp.camera_ray = false;
        ray_out.write(ray_temp);
        attenuation_chan_out.write(tmp_accum_in);
    }
    }

private:
    QuadHit quadInters;

    HitRecord rec_quad;
};

#endif
