#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "RayGeneration.h"
#include "HitRecord.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>
S

class WorldHit {
public:
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design ccore
    // #pragma hls_interface ap_ctrl_none port=return
    bool hit(ac_channel<ray>& ray_in,
             ac_channel<int_11>& num_spheres, // SHOULD I EVEN USE AC CHANNEL OR JUST PASS IN AS INT STILL A BIT UNSURE 
             ac_channel<int_11>& num_quads,
             ac_channel<sphere_hittable>& spheres, /// WHAT IS CORRECT WAY TO PASS IN RAM WANT TO READ 1 BY 1 AND PIPELINE READS; ONLY STORE CLOSEST
             ac_channel<quad_hittable>& quads,
             ac_channel<HitRecord>& hit_out) {

        ray ray_temp = ray_in.read(); 
        int_11 sphere_count = num_spheres.read(); 
        int_11 quad_count = num_quads.read(); 

        HitRecord rec;
        bool hit_anything = false;
        auto closest_so_far = ray_temp.t_max;  // Use t_max from the ray as the initial closest distance



        // WE CAN HAVE THE READS HAPPEN IN PARALLEL THEN DECIDE WHICH ONE WAS THE CLOSEST OUT OF ALL SPHERES AND QUADS
        // MIGHT BE TOO MUCH AREA AND RELIES ON EVEN SPLIT SO PROBABLY NOT

        // Process all spheres
        for (int i = 0; i < sphere_count; i++) { // FIGURE OUT TO LOOP THROUGH WITH STTI VERIBLES IN 272 THEY DID IT WITH VARIABLE ITERATION AMUNT
            if (spheres.available(1)) { // not sure whether to use (this will just check if theres antoher sphere in the buffer or not)
                sphere_hittable sph = spheres.read();
                SphereIntersections sphInters;
                HitRecord temp_rec;
                if (sphInters.hit(ray_temp, closest_so_far , sphere_hittable, temp_rec)) { // again see what the best return type and exact syntax is for this function call
                    hit_anything = true;
                    closest_so_far = temp_rec.t;// Update the closest hit distance.
                    rec = temp_rec; // Update the closest hit distance.
                }
            }
        }
        
        // PROBABLY COMBINE LOOPS TO SERIALIZE AND NOT HAVE TO KEEP TRACK OF 2 VARIABLES

        // Process all quads
        // p much exact same as sph intersection with same questions
        for (int i = 0; i < quad_count; i++) {
            if (quads.available(1)) {
                quad q = quads.read();
                QuadIntersections QuadInters;
                HitRecord temp_rec;
                if (QuadInters.hit(ray_temp, closest_so_far, sphere_hittable, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
        }

        if (hit_anything) {
            hit_out.write(rec);  // Write the closest hit record to the output channel
            return true;
        }

        return false; // Not partiuclary sure how returns work or if this should be an output bool or what
    }
};