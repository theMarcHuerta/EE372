#pragma once

#include "Renderer.h"

template<typename T>
class hit_record {
  public:
    vec3<T> hit_loc;
    vec3<T> normal;
    uint_2 mat;
    T t;
    T u;
    T v;
    bool front_face;
    bool is_secondary_ray;

    hit_record() : hit_loc(vec3<T>()), normal(vec3<T>()), mat(0), t(0), u(0), v(0),
                   front_face(true), is_secondary_ray(false) {}

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void set_face_normal(ray<T>& r, vec3<T>& outward_normal) {
        front_face = (r.dir.dot(outward_normal) < 0);
        normal = front_face ? outward_normal : outward_normal.negate();
    }
};