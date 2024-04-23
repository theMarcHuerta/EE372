#pragma once

#include "RTcore.h"

template<typename T, typename D>
class MetalScatter {
  public:
    MetalScatter() {}

    #pragma hls_design
    void run(ray<T>& r, HitRecord<T>& rec, D& fuzz, ray<T>& scattered, bool& scatter) {
        vec3<T> reflected;
        vec3<T> unit_ray_dir;
        vec3<D> rand_dir;
        vec3<D> fuzzy_rand; // made type D bc it we just add this with reflected and it'all auto extend bit width
        vec3<T> new_ray_dir;

        unit.run(r.dir, normalized_ray_dir);
        reflect.run(unit_ray_dir, rec.normal, reflected);

        rand_unit.run(rand_dir);
        mult.run(rand_dir, fuzz, fuzzy_rand);
        vec3<T> fuzzy_rand_bit_ext;
        fuzzy_rand_bit_ext.x = fuzzy_rand.x;
        fuzzy_rand_bit_ext.y = fuzzy_rand.y;
        fuzzy_rand_bit_ext.z = fuzzy_rand.z;
        add.run(reflected, fuzzy_rand_bit_ext, new_ray_dir);

        ray<T> scattered = {{rec.hit_loc.x, rec.hit_loc.y, rec.hit_loc.z},
                         {new_ray_dir.x, new_ray_dir.y, new_ray_dir.z}};

        T dot_result;
        dot.run(new_ray_dir, rec.normal, dot_result);
        scatter = dot_result > 0;
    }

  private:
    Vec3_add<T> add;
    Vec3_mult_s<D> mult;
    Vec3_dot<T> dot;
    Vec3_reflect<T> reflect;
    Vec3_unit<T> unit;
    Rand_unit_vec<D> rand_unit;
};