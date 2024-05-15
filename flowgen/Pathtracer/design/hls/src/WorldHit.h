#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"
#include "QuadHit.h"
#include "IntersecLoop.h"


class WorldHit {
public:
    WorldHit(){}
    // Function to find the closest hit from channels of quads
    #pragma hls_design interface
    // #pragma hls_pipeline_init_interval 1
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
    
        buffer_obj_count tmp_params = {0,{0,0,0}, false};
        tmp_params = params_in.read();

        ray ray_temp = {{0,0,0},{0,0,0},false};
        ray_temp = ray_in.read(); 
        ray ray_temp_two = ray_temp; 

        rgb_in tmp_color_in = {0,0,0};
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in tmp_accum_in = {0,0,0};
        tmp_accum_in = attenuation_chan_in.read();

        // rgb_in color_tbd_out = {0,0,0};

        bool quad_hit_anything = false;
        bool quad_hit_anything_two = false;
        
        ac_fixed<47, 17, true> closest_so_far_quad = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance
        ac_fixed<47, 17, true> closest_so_far_quad_two = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance

        HitRecord record_out = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
        bool isHit_out = false;
        rgb_in rgb_color_out = {0,0,0};

        // WE CAN HAVE THE READS HAPPEN IN PARALLEL THEN DECIDE WHICH ONE WAS THE CLOSEST OUT OF ALL QUADS
        // MIGHT BE TOO MUCH AREA AND RELIES ON EVEN SPLIT SO PROBABLY NOT
        
        uint_11 quad_max_one = (tmp_params.num_quads) >> 1;
        uint_11 quad_max_two = tmp_params.num_quads - quad_max_one;

        HitRecord rec_quad = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
        HitRecord rec_quad_two = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
        
        // PROBABLY COMBINE LOOPS TO SERIALIZE AND NOT HAVE TO KEEP TRACK OF 2 VARIABLES
        // #pragma hls_pipeline_init_interval 2
        // #pragma hls_unroll 2
        intersec_loop_one.hit(quads, ray_temp, quad_max_one, closest_so_far_quad, quad_hit_anything, rec_quad);
        intersec_loop_two.hit(quads, ray_temp_two, quad_max_two, closest_so_far_quad_two, quad_hit_anything_two, rec_quad_two);

        if (quad_hit_anything | quad_hit_anything_two) {
            // std::cout << "WAS HIT" << std::endl;
            if (closest_so_far_quad < closest_so_far_quad_two){
                record_out = rec_quad;
            }
            else {
                record_out = rec_quad_two;
            }
            isHit_out = true;
            rgb_color_out = tmp_color_in;
        }
        else {
            rgb_in colorMulOut = {0,0,0};

            colorMulOut.r = tmp_accum_in.r * tmp_params.background.r;
            colorMulOut.g = tmp_accum_in.g * tmp_params.background.g;
            colorMulOut.b = tmp_accum_in.b * tmp_params.background.b;

            rgb_in colorAddOut = {0,0,0};

            colorAddOut.r = colorMulOut.r + tmp_color_in.r;
            colorAddOut.g = colorMulOut.g + tmp_color_in.g;
            colorAddOut.b = colorMulOut.b + tmp_color_in.b;

            record_out = empty_rec;
            isHit_out = false;
            rgb_color_out = colorAddOut;
        }
        //any ray going out is not a camera ray anymore

        hit_out.write(record_out);  // Write the closest hit record to the output channel
        isHit.write(isHit_out);
        accumalated_color_out.write(rgb_color_out);
        //////////////////////////////////////////
        ray_temp.camera_ray = false;
        ray_out.write(ray_temp);
        attenuation_chan_out.write(tmp_accum_in);
    }

private:
    const HitRecord empty_rec = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
    IntersecLoop intersec_loop_one;
    IntersecLoop intersec_loop_two;
};

#endif











