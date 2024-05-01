
#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "fixedpoint.h"
#include "vec3.h"
// #include "quad.h"

struct hit_record;  // Forward declaration for usage in material methods.

class lambertian {
  public:
    lambertian(const cpp_vec3& a) : albedo(a){}

    // Implements the scattering function for a diffuse material.
    // Scatters rays in random directions with no reflection.
    bool scatter(const c_ray& r_in, const hit_record& rec, cpp_vec3& attenuation, c_ray& scattered) {
      auto scatter_direction = rec.normal + random_unit_vector();

      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
          scatter_direction = rec.normal;

      //2^-23
      if (std::abs(scatter_direction.x()) < 0.00000011920929) {scatter_direction.e[0] = scatter_direction.x() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.y()) < 0.00000011920929) {scatter_direction.e[1] = scatter_direction.y() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.z()) < 0.00000011920929) {scatter_direction.e[2] = scatter_direction.z() < 0 ? -0.00000011920929 : 0.00000011920929;}
      scatter_direction.e[0] = FixedPoint<23>(scatter_direction.x()).toDouble();
      scatter_direction.e[1] = FixedPoint<23>(scatter_direction.y()).toDouble();
      scatter_direction.e[2] = FixedPoint<23>(scatter_direction.z()).toDouble();

      scattered = c_ray(rec.p, scatter_direction);
      attenuation = albedo;  // The color is affected by the material's albedo.
      return true;
    }

    cpp_vec3 albedo;  // Represents the color and intensity of the scattered light.
    int matnumber = 0;
};

#endif
