#pragma once

#include "RTcore.h"

struct quad_hittable {
    vec3<int_11> corner_pt; // for quads its corner
    vec3<int_11> u; // defining u component 
    vec3<int_11> v; // defining v componenet
    uint_2 mat_type; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??
    pbool is_invis;
    vec3<int_11> normal; // cross of u and v
    vec3<int_11> w; // dot of u and v
    int_12 d_plane;
    rgb_t quad_color;
};

// create an additional templatated version for the struct to allow for arithmetic in function, without screwing
// up I/O
template<typename T>
struct _quad_hittable {
    vec3<T> corner_pt;
    vec3<T> u; // defining u component 
    vec3<T> v; // defining v componenet
    uint_2 mat_type; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??
    pbool is_invis;
    vec3<T> normal; // cross of u and v
    vec3<T> w; // dot of u and v
    T d_plane;
    rgb_t quad_color;
};

template<typename T>
class QuadHit {
  private:
    Vec3_sub<T> sub;
    Vec3_dot<T> dot;
    Vec3_cross<T> cross;
    Ray_at<T> at;
  public:
    QuadHit() {}

    #pragma hls_design
    bool run(ray<T>& r, T& closest_so_far, quad_hittable& quad, HitRecord<T>& rec) {
        // // read in data from channels
        // Ray r = rays.read();
        // T closest = closest_so_far.read();
        // quad_hittable quad = quads.read();
        // HitRecord rec = recs.read();

        // create quad hittable with compatible bit widths for arithmetic
        _quad_hittable<T> _quad = {{quad.corner_pt.x, quad.corner_pt.y, quad.corner_pt.z},
                                   {quad.u.x, quad.u.y, quad.u.z}, {quad.v.x, quad.v.y, quad.v.z},
                                    quad.mat_type, quad.is_invis, {quad.normal.x, quad.normal.y, quad.normal.z},
                                    {quad.w.x, quad.w.y, quad.w.z}, quad.d_plane,
                                    {quad.quad_color.r, quad.quad_color.g, quad.quad_color.b}};

        T denom;
        dot.run(_quad.normal, r.dir, denom);

        if (denom == 0)  // Check for parallel ray TO DO MAKE FIXED POINT REP FOR 1e-8
        //  absolute value of a floating-point number is what fabs is so we have to implement that
            return false;

        T x;
        dot.run(_quad.normal, r.orig, x);

        T t = (_quad.d_plane - x) / denom; // initate correct dot product format and all
        if (t < 0 || t > closest_so_far)  // Check if t is within the valid interval
            return false;

        // Calculate intersection point
        vec3<T> intersection;
        at.run(r, t, intersection);

        // Convert point to planar coordinates relative to the quad
        vec3<T> planar_hitpt_vector;
        sub.run(intersection, _quad.corner_pt, planar_hitpt_vector);

        vec3<T> cross_0;
        T alpha;

        vec3<T> cross_1;
        T beta;

        cross.run(planar_hitpt_vector, _quad.v, cross_0);
        dot.run(_quad.w, cross_0, alpha);

        cross.run(_quad.u, planar_hitpt_vector, cross_1);
        dot.run(_quad.w, cross_1, beta);

        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true
        if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1)  // Check if inside quad bounds
            return false;

        rec.u = alpha;
        rec.v = beta;
        rec.t = t;
        rec.hit_loc = intersection;
        rec.color = _quad.quad_color;

        HitRecord_setNorm<T> setfacenorm; 
        setfacenorm.run(r, _quad.normal, rec);
        rec.mat = _quad.mat_type;

        closest_so_far = t;  // Update the closest hit
        return true;
    }
};