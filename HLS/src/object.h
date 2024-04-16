#pragma once

#include "Renderer.h"

template<typename T>
class object {
  public:
    pbool type;  // 0 for quad, 1 for sphere

    // parameters which can be shared between object type
    vec3<T> q_or_center;
    T d_or_radius;
    uint_2 mat;

    // extra parameters needed for a quad type object
    vec3<T> u, v;
    vec3<T> normal;
    vec3<T> w;

    object(pbool _type, vec3<T> _q, T _d, uint_2 _m, vec3<T> _u, vec3<T>_v) :
           q_or_center(_q), d_or_radius(_d), mat(_m), u(_u), v(_v) {

        // if object is a quad, do further initialization
        if (_type == QUAD) {
            vec3<T> n = u.cross(v);
            normal = n.unit();
            d_or_radius = normal.dot(q_or_center);
            T n_dot = n.dot(n);
            w = n.div(n_dot);
        }
    }

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    bool quad_is_interior(T& a, T& b, hit_record& rec) {
        if ((a < 0) || (a > 1) || (b < 0) || (b > 1)) return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

    bool quad_hit(ray<T>& r, interval<T> ray_t, hit_record& rec) {
        // TODO
        return false;
    }

    bool sphere_hit(ray<T>& r, interval<T> ray_t, hit_record& rec) {
        // TODO
        return false;
    }

    bool hit(ray<T>& r, interval<T> ray_t, hit_record& rec) {
        if (type == QUAD) {
            quad_hit(r, ray_t, rec);
        } else if (type == SPHERE) {
            sphere_hit(r, ray_t, rec);
        } else {
            return false;
        }
    }
    
};