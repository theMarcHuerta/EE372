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



class QuadHit {
public:
    #pragma hls_design ccore
    bool hit(const ray& r, sfp_11_22& closest_so_far, const quad_hittable& quad, hit_record& rec) {
        sfp_11_22 denom = dot(quad.normal, r.direction);

        if (fabs(denom) < 1e-8)  // Check for parallel ray TO DO MAKE FIXED POINT REP FOR 1e-8
        //  absolute value of a floating-point number is what fabs is so we have to implement that
            return false;

        sfp_11_22 t = (quad.D - dot(quad.normal, r.origin)) / denom; // initate correct dot product format and all
        if (t < 0 || t > closest_so_far)  // Check if t is within the valid interval
            return false;

        // Calculate intersection point
        ws_point intersection;
        intersection.x = r.origin.x + r.direction.x * t;
        intersection.y = r.origin.y + r.direction.y * t;
        intersection.z = r.origin.z + r.direction.z * t;

        // Convert point to planar coordinates relative to the quad
        full_vec3 planar_hitpt_vector = {intersection.x - quad.Q.x, intersection.y - quad.Q.y, intersection.z - quad.Q.z};
        sfp_11_22 alpha = dot(quad.w, cross(planar_hitpt_vector, quad.v));
        sfp_11_22 beta = dot(quad.w, cross(quad.u, planar_hitpt_vector));


        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true
        if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1)  // Check if inside quad bounds
            return false;

        rec.u = a;
        rec.v = b;
        rec.t = t;
        rec.p = intersection;
        rec.color = quad.quad_color;
        HitRecordPhysics setfacenorm; 
        setfacenorm.setFaceNormal(rec,r, outward_normal);
        rec.mat_type = quad.mat_type;

        closest_so_far = t;  // Update the closest hit
        return true;
    }
};