#ifndef QUADHIT_H
#define QUADHIT_H

#include "RTcore.h"
#include "vec3.h"
#include "ray.h"
#include "HitRecord.h"
#include <math/mgc_ac_math.h>
using namespace ac_math;

class QuadHit {

  public:
    QuadHit() {}

    #pragma hls_design
    void run(ray& r, ac_fixed<47, 17, true>& closest_so_far, quad_hittable& _quad, HitRecord& rec, bool& hitWorld) {        

        if (r.camera_ray && _quad.is_invis){
            hitWorld = false;
            rec = {{0,0,0}, {0,0,0}, 0, 0, {0, 0, 0}};
            return;
        }

        ac_fixed<58, 11, true> denom = 0; //47 fractional bits out as result, 24*23 fractioanl bit and tested 11 int bits largest value gotten in c test
        denom_dot.run(_quad.normal, r.dir, denom);
        ac_fixed<58, 11, true> abs_denom = 0; // gets rid of sign bit
        ac_math::ac_abs(denom, abs_denom);

        //new type will be 11_23
        ac_fixed<34, 11, true> rounded_denom = denom; // HAVE TO CHECK IF IT TRUNCATES THE INTEGER OR FRACTIONAL BITS ON IT'S OWN
        // checks if denom was negative and saturates the rounded denom
        ac_fixed<24,1, true> rounding_val = 1.1920929e-7; // 2^-23-- hope it sets lowest bit
        ac_fixed<24,1, true> rounding_val_neg = -1.1920929e-7;
        if (abs_denom < rounding_val) {rounded_denom = (denom[57] == 1) ? rounding_val_neg : rounding_val;}

        // T abs_denom;
        // ac_math::ac_abs(rounded_denom, abs_denom);

        ac_fixed<24,1, true> parallel_check = 9.53674316e-7; // 2^-20
        ac_fixed<33, 10, false> abs_rounded_denom = 0;
        ac_math::ac_abs(rounded_denom, abs_rounded_denom);
        if (abs_denom < parallel_check)  {// Check for parallel ray TO DO MAKE FIXED POINT REP FOR 1e-8
        //  absolute value of a floating-point number is what fabs is so we have to implement that
            hitWorld = false;
            rec = {{0,0,0}, {0,0,0}, 0, 0, {0, 0, 0}};
            return;
        }

        ac_fixed<49, 14, true> x = 0; //added 2 overflow bits to results
        qnorm_rorig.run(_quad.normal, r.orig, x);
        // now truncate to 20 fractioanl bits
        ac_fixed<34, 14, true> x_trunc = x;

        //20fractioanl bits + 23 fractional bits from denom = 43 fractional bits from result at least, 43+17
        ac_fixed<34, 14, true> sub_result = (_quad.d_plane - x_trunc);

        // ac_fixed<60, 17, true> t = sub_result / rounded_denom; // initate correct dot product format and all
        ac_fixed<60, 17, true> t = 0;
        div(sub_result, rounded_denom, t);

        ac_fixed<47, 17, true> t_trunc = t; 
        // now truncate fractional bits to 30 
        ac_fixed<31,1, true> rounding_val2 = 9.3132257e-10; // 2^-30 - hope it sets lowest bit
        ac_fixed<31,1, true> rounding_val2_neg = -9.3132257e-10;
        if (t < rounding_val2) {t_trunc = t[59] == 1 ? rounding_val2_neg : rounding_val2;}

        ac_fixed<11,1,true> min_val = 0.0009765625; // 2^-10 approx 0.001
        // can there be overflow with different types of compos or will it auto adjust the comparison to the largest number 
        if (t_trunc < min_val || t_trunc > closest_so_far)  {// Check if t is within the valid interval
            hitWorld = false;
            rec = {{0,0,0}, {0,0,0}, 0, 0, {0, 0, 0}};
            return;
        }

        // Calculate intersection point
        vec3<ac_fixed<75, 22, true>>  intersection = {0, 0, 0};
        at.run(r, t_trunc, intersection);

        vec3<ac_fixed<45, 22, true>> trunc_intersection = {0, 0, 0};
        trunc_intersection.x = intersection.x;
        trunc_intersection.y = intersection.y;
        trunc_intersection.z = intersection.z;
        // Convert point to planar coordinates relative to the quad
        vec3<ac_fixed<45, 22, true>> planar_hitpt_vector = {0, 0, 0};
        sub.run(trunc_intersection, _quad.corner_pt, planar_hitpt_vector);

        vec3<ac_fixed<56, 33, true>> cross_v_result = {0, 0, 0};
        cross_v.run(planar_hitpt_vector, _quad.v, cross_v_result);

        vec3<ac_fixed<56, 33, true>> cross_u_result = {0, 0, 0};
        cross_u.run(_quad.u, planar_hitpt_vector, cross_u_result);

        ac_fixed<38, 15, true> alpha = 0;
        ac_fixed<38, 15, true> beta = 0;
        
        alpha_dot.run(_quad.w, cross_v_result, alpha);
        beta_dot.run(_quad.w, cross_u_result, beta);

        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true
        if ((alpha < 0) || (alpha > 1) || (beta < 0) || (beta > 1)) { // Check if inside quad bounds
            hitWorld = false;
            rec = {{0,0,0}, {0,0,0}, 0, 0, {0, 0, 0}};
            return;

        }

        ac_fixed<11,1, true> rounding_val3 = 0.0009765625; // 2^-23-- hope it sets lowest bit
        ac_fixed<11,1, true> rounding_val_neg3 = -0.0009765625;
        
        rec.hit_loc.x = trunc_intersection.x; // HAVE TO HOPE IT TRUNCS THIS TOO
        rec.hit_loc.y = trunc_intersection.y;
        rec.hit_loc.z = trunc_intersection.z;
        
        ac_fixed<21, 11, true> abs_x = 0; // gets rid of sign bit
        ac_math::ac_abs(rec.hit_loc.x, abs_x);

        ac_fixed<21, 11, true> abs_y = 0; // gets rid of sign bit
        ac_math::ac_abs(rec.hit_loc.y, abs_y);

        ac_fixed<21, 11, true> abs_z = 0; // gets rid of sign bit
        ac_math::ac_abs(rec.hit_loc.z, abs_z);

        if (abs_x < rounding_val3) {rec.hit_loc.x = (intersection.x[74] == 1) ? rounding_val_neg3 : rounding_val3;} 
        // std::cout << ((intersection.x)) << std::endl << ((t_trunc)) << std::endl  << ((r.dir.x))<<  std::endl << ((r.orig.x)) << std::endl << std::endl;}
        if (abs_y < rounding_val3) {rec.hit_loc.y = (intersection.y[74] == 1) ? rounding_val_neg3 : rounding_val3;}
        if (abs_z < rounding_val3) {rec.hit_loc.z = (intersection.z[74] == 1) ? rounding_val_neg3 : rounding_val3;}

        rec.color = _quad.quad_color;

        // HitRecord_setNorm<T> setfacenorm; 
        setfacenorm.run(r, _quad.normal, rec);
        rec.mat = _quad.mat_type;

        closest_so_far = t_trunc;  // Update the closest hit // HOPE IT TRUNCATES LARGEST 6 BITS
        hitWorld = true;
    }

private:
    Vec3_sub<ac_fixed<45, 22, true>, ac_fixed<12, 11, true>, ac_fixed<45, 22, true>> sub;

    Vec3_dot<ac_fixed<26, 2, true>, ac_fixed<34, 11, true>, ac_fixed<58, 11, true>> denom_dot;
    Vec3_dot<ac_fixed<26, 2, true>, ac_fixed<21, 11, true>, ac_fixed<49, 14, true>> qnorm_rorig; //added 2 overflow bits to results
    Vec3_dot<ac_fixed<25, 1, true>, ac_fixed<56, 33, true>, ac_fixed<38, 15, true>> alpha_dot; //added 2 overflow bits to results
    Vec3_dot<ac_fixed<25, 1, true>, ac_fixed<56, 33, true>, ac_fixed<38, 15, true>> beta_dot; //added 2 overflow bits to results

    Vec3_cross<ac_fixed<45, 22, true>, ac_fixed<12, 11, true>, ac_fixed<56, 33, true>> cross_v;
    Vec3_cross<ac_fixed<12, 11, true>, ac_fixed<45, 22, true>, ac_fixed<56, 33, true>> cross_u;
    Ray_at<ac_fixed<47, 17, true>, ac_fixed<75, 22, true>> at; // first val + ray dir frac bits// capped at 22 int bits according to c

    HitRecord_setNorm setfacenorm;  // type is size of normla of 
};

#endif