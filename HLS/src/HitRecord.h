#pragma once

#include "RTcore.h"

template<typename T>
struct HitRecord {
    vec3<T> hit_loc;
    vec3<T> normal;
    bool front_face;
    T t; // how far along the hit occured
    T u;
    T v;
    uint_2 mat;
    rgb_t color;

};

template<typename T>
class HitRecord_setNorm {
  Vec3_dot<T> dot;
  Vec3_negate<T> negate;
  public:

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(ray<T>& r, vec3<T>& outwardNormal, HitRecord<T>& record) {
        T dot_result;
        dot.run(r.dir, outwardNormal, dot_result);
        vec3<T> negated_normal;
        negate.run(outwardNormal, negated_normal);
        record.front_face = (dot_result < 0);
        record.normal = record.front_face ? outwardNormal : negated_normal;
    }
};
