#pragma once

#include "RTcore.h"

template<typename T>
class MetalScatter {
  public:
    MetalScatter() {}

    #pragma hls_design
    void run(ray<T>& r, HitRecord<T>& rec, T& fuzz, ray<T>& scattered, bool& scatter) {
        vec3<T> reflected;
        vec3<T> unit_ray_dir;
        vec3<T> rand_dir;
        vec3<T> fuzzy_rand;
        vec3<T> new_ray_dir;

        unit.run(r.dir, normalized_ray_dir);
        reflect.run(unit_ray_dir, rec.normal, reflected);

        rand_unit.run(rand_dir);
        mult.run(rand_dir, fuzz, fuzzy_rand);
        add.run(reflected, fuzzy_rand, new_ray_dir);

        ray<T> scattered = {{rec.hit_loc.x, rec.hit_loc.y, rec.hit_loc.z},
                         {new_ray_dir.x, new_ray_dir.y, new_ray_dir.z}};

        attenuation = albedo;

        T dot_result;
        dot.run(new_ray_dir, rec.normal, dot_result);
        scatter = dot_result > 0;
    }

  private:
    Vec3_add<T> add;
    Vec3_mult_s<T> mult;
    Vec3_dot<T> dot;
    Vec3_reflect<T> reflect;
    Vec3_unit<T> unit;
    Rand_unit_vec<T> rand_unit;
};