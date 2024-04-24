#pragma once

#include "RTcore.h"

template<typename T>
class LambertianScatter {
  public:
    LambertianScatter() {}

    #pragma hls_design
    void run(HitRecord<T>& rec, ray<T>& scattered) {
        vec3<T> rand_dir;
        vec3<T> scatter_direction;

        rand_unit.run(rand_dir);
        add.run(rec.normal, rand_dir, scatter_direction);

        if ((scatter_direction.x == 0) && (scatter_direction.y == 0) && (scatter_direction.z == 0)) {
            scatter_direction = rec.normal;
        }

        scattered = {{rec.hit_loc.x, rec.hit_loc.y, rec.hit_loc.z},
                         {scatter_direction.x, scatter_direction.y, scatter_direction.z}};

    }

  private:
    Vec3_add<T> add;
    Rand_unit_vec<T> rand_unit;
};
