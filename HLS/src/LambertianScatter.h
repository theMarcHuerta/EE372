#pragma once

#include "RTcore.h"


class LambertianScatter {
  public:
    LambertianScatter() {}

    #pragma hls_design
    void run(HitRecord& rec, ray& scattered) {
        vec3<ac_fixed<34, 2, true>> rand_dir;
        vec3<ac_fixed<34, 3, true>> scatter_direction;

        rand_unit.run(rand_dir);
        add.run(rec.normal, rand_dir, scatter_direction);

        vec3<ac_fixed<34, 2, true>> near_zero_val = 1.49011612e-8;
        if ((scatter_direction.x < near_zero_val) && (scatter_direction.y < near_zero_val) && (scatter_direction.z < near_zero_val)) {
            scatter_direction = rec.normal;
        }

        // LETS SEE IF IT TRUNCATES TO RIGHT SIZE OTHERWISE WE HAVE TO MANUALLY SET IT
        scattered = {{rec.hit_loc.x, rec.hit_loc.y, rec.hit_loc.z},
                         {scatter_direction.x, scatter_direction.y, scatter_direction.z}};

    }

  private:
  // rec normal size, rand dir size, resulting size
    Vec3_add<ac_fixed<26, 2, true>, ac_fixed<34, 2, true>, ac_fixed<34, 3, true>> add;
    Rand_unit_vec rand_unit;
};
