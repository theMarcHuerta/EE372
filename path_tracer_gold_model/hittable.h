#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"          // Includes the definition of a ray.
#include "rtweekend.h"    // Includes common utilities and constants.


struct hit_record {
  point3 p;           // The point in 3D space where the ray hits the object.
  vec3 normal;        // The normal vector at the hit point, pointing outward.
  int mat;  // Pointer to the material of the object. Determines how the surface interacts with light.
  bool front_face;    // True if the ray hits the front face of the object, false if the back.
  bool is_secondary_ray = false;
  color mat_color;
  // Determines the correct orientation for the normal vector. Ensures normal points against the ray.
  void set_face_normal(const ray& r, const vec3& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
  }
};

#endif
