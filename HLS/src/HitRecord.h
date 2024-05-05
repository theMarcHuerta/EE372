#ifndef HITRECORD_H
#define HITRECORD_H

#include "RTcore.h"


class HitRecord_setNorm {
  Vec3_dot<ac_fixed<34, 11, true>, ac_fixed<26, 2, true>, ac_fixed<60, 13, true>> dot;
  Vec3_negate<ac_fixed<26, 2, true>> negate;
  public:

    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(ray& r, vec3<ac_fixed<26, 2, true>>& outwardNormal, HitRecord& record) {
        ac_fixed<60, 13, true> dot_result = 0; // can truncate this later 
        dot.run(r.dir, outwardNormal, dot_result);
        vec3<ac_fixed<26, 2, true>> negated_normal = {0, 0, 0};
        negate.run(outwardNormal, negated_normal);
        record.front_face = (dot_result[59] == 1);
        // record.front_face = (dot_result < 0);
        record.normal = record.front_face ? outwardNormal : negated_normal;
    }
};

#endif
