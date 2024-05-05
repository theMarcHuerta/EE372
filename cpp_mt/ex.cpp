#pragma once

#include "RTcore.h"

struct sphere_hittable {
    vec3<int_11> center; // for quads its corner, sphere it's center
    uint_8 radius; // radius cant clip edge of range of ws view
    uint_3 mat_type; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??
    rgb_in sph_color;
};

// create an additional templatated version for the struct to allow for arithmetic in function, without screwing
// up I/O
template<typename T>
struct _sphere_hittable {
    vec3<T> center;
    T radius;
    uint_2 mat_type;
    rgb_in sph_color;
};

// type is base, toverflow 1 bit wider, t mult is 2x widermm tmultadd 2x + 2 bits to account for 3 adds of type tmultadd
template<typename T, typename Toverflow, typename Tmultadd>
class SphereHit {
  private:
    Vec3_sub<T> sub;
    Vec3_div_s<T> div;
    Vec3_len_sq<T, Tmultadd> len_squared;
    Vec3_dot<Tmultadd> dot;
    Ray_at<T> ray_at;
  public:
    SphereHit() {}

    // Method to determine if a ray intersects with a sphere
    #pragma hls_design ccore
    bool run(ray<T>& r, Toverflow& closest_so_far, sphere_hittable& sphere, HitRecord<T>& rec) { // NOT SURE WHETHER TO AC CHANNEL THE SPHERE HITABLE OR IF THE LOOP TAKES CARE OF THIS HMMM

        // create sphere hittable with compatible bit widths for arithmetic
        _sphere_hittable<T> _sphere = {{sphere.center.x, sphere.center.y, sphere.center.z},
                                        sphere.radius, sphere.mat_type};

        vec3<T> oc;  // Vector from ray orig to sphere center
        sub.run(r.orig, _sphere.center, oc);

        Tmultadd a;
        len_squared.run(r.dir, a); 
        
        Tmultadd half_b;
        dot.run(oc, r.dir, half_b);

        Tmultadd len_sq_oc;
        len_squared.run(oc, len_sq_oc);

        Tmultadd c = len_sq_oc - (_sphere.radius * _sphere.radius);

        // doubles the integer width since were squaring half_b
        ac_fixed<half_b.width + half_b.i_width, half_b.i_width * 2, true> discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;  // No intersection// ALSO SHOULD I MAKE THE COMPARIOSN FIXED POINT OR WHAT?

        ac_fixed<discriminant.width-1, discriminant.i_width-1, false> u_discriminant = discriminant; // unsigned fixed point relative to type T for ac_sqrt
        ac_fixed<discriminant.width-1, discriminant.i_width-1, false> u_sqrtd;
        ac_math::ac_sqrt(u_discriminant, u_sqrtd);
        Tmultadd sqrtd = u_sqrtd;  // convert back to type T

        Toverflow root = (-half_b - sqrtd) / a;

        if (!((root > SMALLEST_INTERVAL) && (root < closest_so_far))) {
            root = (-half_b + sqrtd) / a;
            if (!((root > SMALLEST_INTERVAL) && (root < closest_so_far)))
                return false;
        }

        rec.t = root;

        ray_at.run(r, root, rec.hit_loc);
        rec.color = sphere.sph_color;

        vec3<T> outward_normal;
        vec3<T> sub_result;
        sub.run(rec.hit_loc, _sphere.center, sub_result);
        div.run(sub_result, _sphere.radius, outward_normal);

        // bool front_face = dot(r.direction, outward_normal) < 0; // ALSO HAVE A FUNCTION FO RTHIS IN HITRECORD BUT ITS A CLASS MIGHT JUST MAKE IT A FUNCTION
        HitRecord_setNorm<T> setfacenorm; 
        // rec.normal = front_face ? outward_normal : full_vec3{-outward_normal.x, -outward_normal.y, -outward_normal.z};
        setfacenorm.run(r, outward_normal, rec);

        rec.mat = _sphere.mat_type;

        closest_so_far = root;  // Update the closest_so_far

        return true;
    }
};


#pragma once

#include "RTcore.h"

template<typename T>
struct ray {
  vec3<T> orig; 
  vec3<T> dir;
};

template<typename T, typename Tmul, typename Tmuladd>
class Ray_at {
  Vec3_mult_s<T, Tmul> mult;
  Vec3_add<T, Tmuladd> add;
  public:
    // Calculates a point along the ray at parameter t.
    #pragma hls_design ccore
    #pragma hls_ccore_type combinational
    void run(ray<T>& r, T& t, vec3<Tmuladd>& out) {
        vec3<Tout> mult_result;
        mult.run(r.dir, t, mult_result);
        Tmul tmp_orig = r.orig;
        add.run(tmp_orig, mult_result, out);
    }
};    