#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include "RTcore.h"
#include "ray.h"
#include "HitRecord.h"
#include "QuadHit.h"
#include "SphHit.h"


template<typename T, typename D>
class WorldHit {
public:
    WorldHit(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design ccore
    // #pragma hls_interface ap_ctrl_none port=return
    void CCS_BLOCK(hit)(ac_channel<ray<T>>& ray_in,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<sphere_hittable>& spheres, /// WHAT IS CORRECT WAY TO PASS IN RAM WANT TO READ 1 BY 1 AND PIPELINE READS; ONLY STORE CLOSEST
             ac_channel<quad_hittable>& quads,
             ac_channel<rgb_in> &attenuation_chan_in,
             ac_channel<rgb_in> &accumalated_color_chan_in,
             ac_channel<rgb_in> &attenuation_chan_out,
             ac_channel<rgb_in> &accumalated_color_out,
             ac_channel<HitRecord<T>>& hit_out,
             ac_channel<ray<T>> &ray_out,
             ac_channel<bool> &isHit,
             ) 
    {

        buffer_obj_count tmp_params;
        tmp_params = params_in.read();

        ray<T> ray_temp;
        ray_temp = ray_in.read(); 

        rgb_in tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in tmp_accum_in;
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
            HitRecord<T> temp_rec;
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
            HitRecord<T> temp_rec;
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
            rgb_in colorMulOut;
            colorMul.run(tmp_accum_in, tmp_params.background, colorMulOut);
            rgb_in colorAddOut;
            colorAdd.run(colorMulOut, tmp_color_in, colorAddOut);
            accumalated_color_out.write(colorAddOut);
            isHit.write(false);
            hit_out.write(rec_quad);  // doesnt really matter which one we write out as long as we write somethign out
        }

        ray_out.write(ray_temp);
        attenuation_chan_out.write(tmp_accum_in);

    }

private:
    SphereHit<T> sphInters;
    QuadHit<T> quadInters;
    Vec3_mult_s<D> colorMul;
    Vec3_add<D> colorAdd;

    HitRecord<T> rec_sphere;
    HitRecord<T> rec_quad;
};

#endif
