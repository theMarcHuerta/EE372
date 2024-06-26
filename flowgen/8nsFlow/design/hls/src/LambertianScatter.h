#pragma once

#include "RTcore.h"
#include "Rand.h"


class LambertianScatter {
  public:
    LambertianScatter() {
      state1=375821;
      state2=39251088;
    }

    #pragma hls_design
    void run(HitRecord& rec, ray& scattered) {
        vec3<ac_fixed<34, 2, true>> rand_dir;
        vec3<ac_fixed<35, 3, true>> scatter_direction;

        rand_unit.run(state1, state2, rand_dir);
        add.run(rec.normal, rand_dir, scatter_direction);

        ac_fixed<34, 2, false> near_zero_val = 1.49011612e-8; // 2^-26

        ac_fixed<34, 2, false> xx;
        ac_fixed<34, 2, false> yy;
        ac_fixed<34, 2, false> zz;
        ac_math::ac_abs(scatter_direction.x, xx);
        ac_math::ac_abs(scatter_direction.y, yy);
        ac_math::ac_abs(scatter_direction.z, zz);
        if ((xx < near_zero_val) && (yy < near_zero_val) && (zz < near_zero_val)) {
            scatter_direction.x = rec.normal.x;
            scatter_direction.y = rec.normal.y;
            scatter_direction.z = rec.normal.z;
        }

        // LETS SEE IF IT TRUNCATES TO RIGHT SIZE OTHERWISE WE HAVE TO MANUALLY SET IT
        scattered = {{rec.hit_loc.x, rec.hit_loc.y, rec.hit_loc.z},
                         {scatter_direction.x, scatter_direction.y, scatter_direction.z}, false};

    }

  private:
  // rec normal size, rand dir size, resulting size
    Vec3_add<ac_fixed<26, 2, true>, ac_fixed<34, 2, true>, ac_fixed<35, 3, true>> add;
    random_in_unit_sphere rand_unit;
    ac_int<32, false> state1;
    ac_int<32, false> state2;
};
