#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "HitRecord.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class WorldHit {
public:
    WorldHit(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design ccore
    // #pragma hls_interface ap_ctrl_none port=return
    void CCS_BLOCK(hit)(ac_channel<ray<sfp_11_22>>& ray_in,
             ac_channel<img_params> &params_in,
             ac_channel<sphere_hittable>& spheres, /// WHAT IS CORRECT WAY TO PASS IN RAM WANT TO READ 1 BY 1 AND PIPELINE READS; ONLY STORE CLOSEST
             ac_channel<quad_hittable>& quads,
             ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_in,
             ac_channel<rgb_t<sfp_11_10>> &accumalated_color_chan_in,
             ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_out,
             ac_channel<rgb_t<sfp_11_10>> &accumalated_color_out,
             ac_channel<HitRecord<sfp_11_22>>& hit_out,
             ac_channel<ray<sfp_11_22>> &ray_out,
             ac_channel<bool> &isHit,
             ac_channel<img_params> &params_out
             ) 
    {

        img_params tmp_params;
        tmp_params = params_in.read();

        ray<sfp_11_22> ray_temp;
        ray_temp = ray_in.read(); 

        rgb_t<sfp_9_10> tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_t<sfp_11_10> tmp_accum_in;
        tmp_accum_in = attenuation_chan_in.read();

        bool sph_hit_anything = false;
        bool quad_hit_anything = false;
        fp_12_22 closest_so_far_sph = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance
        fp_12_22 closest_so_far_quad = LONGEST_DISTANCE;  // Use t_max from the ray as the initial closest distance

        // WE CAN HAVE THE READS HAPPEN IN PARALLEL THEN DECIDE WHICH ONE WAS THE CLOSEST OUT OF ALL SPHERES AND QUADS
        // MIGHT BE TOO MUCH AREA AND RELIES ON EVEN SPLIT SO PROBABLY NOT

        // Process all spheres
        for (int i = 0; i < tmp_params.num_spheres; i++) { // FIGURE OUT TO LOOP THROUGH WITH STTI VERIBLES IN 272 THEY DID IT WITH VARIABLE ITERATION AMUNT
            sphere_hittable sph = spheres.read();
            HitRecord temp_rec;
            if (sphInters.hit(ray_temp, closest_so_far_sph , sph, temp_rec)) { // again see what the best return type and exact syntax is for this function call
                sph_hit_anything = true;
                closest_so_far_sph = temp_rec.t;// Update the closest hit distance.
                rec_sphere = temp_rec; // Update the closest hit distance.
            }
        }
        
        // PROBABLY COMBINE LOOPS TO SERIALIZE AND NOT HAVE TO KEEP TRACK OF 2 VARIABLES

        // Process all quads
        // p much exact same as sph intersection with same questions
        for (int i = 0; i < tmp_params.num_quads; i++) {
            quad_hittable quad = quads.read();
            HitRecord temp_rec;
            if (quadInters.hit(ray_temp, closest_so_far_quad, quad, temp_rec)) {
                quad_hit_anything = true;
                closest_so_far_quad = temp_rec.t;
                rec_quad = temp_rec;
            }
        }

        if (quad_hit_anything || sph_hit_anything) {
            if (closest_so_far_sph < closest_so_far_quad){
                hit_out.write(rec_sphere);  // Write the closest hit record to the output channel
            }
            else {
                hit_out.write(rec_quad);  // Write the closest hit record to the output channel
            }
            isHit.write(true);
            accumalated_color_out.write(tmp_color_in);
        }
        else {
            sfp_11_10 colorMulOut;
            colorMul.run(tmp_accum_in, tmp_params.background, colorMulOut);
            sfp_11_10 colorAddOut;
            colorAdd.run(colorMulOut, tmp_color_in, colorAddOut);
            accumalated_color_out.write(colorAddOut);
            isHit.write(false);
        }

        accumalated_color_out.write(tmp_accum_in);
        ray_out.write(ray_temp);

    }

private:
    SphereHit<sfp_11_22> sphInters;
    QuadHit<sfp_11_22> quadInters;
    Vec3_mult_s<sfp_11_10> colorMul;
    Vec3_add<sfp_11_10> colorAdd;

    HitRecord<sfp_11_22> rec_sphere;
    HitRecord<sfp_11_22> rec_quad;
};